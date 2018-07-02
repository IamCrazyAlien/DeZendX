#include "php.h"
#include "php_ini.h"
#include "php_win32_globals.h"

#define PHP_REGISTRY_KEY              "SOFTWARE\\PHP"

#define PHP_VER1(V1)                  #V1
#define PHP_VER2(V1,V2)               #V1"."#V2
#define PHP_VER3(V1,V2,V3)            #V1"."#V2"."#V3

#define PHP_REGISTRY_KEYV(VER)        PHP_REGISTRY_KEY"\\"VER
#define PHP_REGISTRY_KEY1(V1)         PHP_REGISTRY_KEY"\\"PHP_VER1(V1)
#define PHP_REGISTRY_KEY2(V1,V2)      PHP_REGISTRY_KEY"\\"PHP_VER2(V1,V2)
#define PHP_REGISTRY_KEY3(V1,V2,V3)   PHP_REGISTRY_KEY"\\"PHP_VER3(V1,V2,V3)

static const char* registry_keys[] = {
	PHP_REGISTRY_KEYV(PHP_VERSION),
	PHP_REGISTRY_KEY3(PHP_MAJOR_VERSION, PHP_MINOR_VERSION, PHP_RELEASE_VERSION),
	PHP_REGISTRY_KEY2(PHP_MAJOR_VERSION, PHP_MINOR_VERSION),
	PHP_REGISTRY_KEY1(PHP_MAJOR_VERSION),
	PHP_REGISTRY_KEY,
	NULL
};

static int OpenPhpRegistryKey(char* sub_key, HKEY *hKey)
{
	const char **key_name = registry_keys;

	if (sub_key) {
		int main_key_len;
		int sub_key_len = strlen(sub_key);
		char *reg_key;

		while (*key_name) {
			LONG ret;

			main_key_len = strlen(*key_name);
			reg_key = emalloc(main_key_len + sub_key_len + 1);
			memcpy(reg_key, *key_name, main_key_len);
			memcpy(reg_key + main_key_len, sub_key, sub_key_len + 1);			
			ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, reg_key, 0, KEY_READ, hKey);
			efree(reg_key);
			
			if (ret == ERROR_SUCCESS) {
				return 1;
			}
			++key_name;
		}
	} else {
		while (*key_name) {
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, *key_name, 0, KEY_READ, hKey) == ERROR_SUCCESS) {
				return 1;
			}
			++key_name;
		}
	}
	return 0;
}

static int LoadDirectory(HashTable *directories, HKEY key, char *path, int path_len, HashTable *parent_ht)
{
	DWORD keys, values, max_key, max_name, max_value;
	int ret = 0;
	HashTable *ht = NULL;

	if (RegQueryInfoKey(key, NULL, NULL, NULL, &keys, &max_key, NULL, &values, &max_name, &max_value, NULL, NULL) == ERROR_SUCCESS) {
		
		if (values) {
			DWORD i;
			char *name = (char*)emalloc(max_name+1);
			char *value = (char*)emalloc(max_value+1);
			DWORD name_len, type, value_len;
			zval *data;

			for (i = 0; i < values; i++) {
				name_len = max_name+1;
				value_len = max_value+1;
				if (RegEnumValue(key, i, name, &name_len, NULL, &type, value, &value_len) == ERROR_SUCCESS) {
					if ((type == REG_SZ) || (type == REG_EXPAND_SZ)) {
						if (!ht) {
							ht = (HashTable*)malloc(sizeof(HashTable));
							zend_hash_init(ht, 0, NULL, ZVAL_INTERNAL_PTR_DTOR, 1);
						}
						data = (zval*)malloc(sizeof(zval));
						INIT_PZVAL(data);
						Z_STRVAL_P(data) = zend_strndup(value, value_len-1);
						Z_STRLEN_P(data) = value_len-1;
						zend_hash_update(ht, name, name_len+1, &data, sizeof(zval*), NULL);
					}
				}
			}
			if (ht) {
				if (parent_ht) {
					HashPosition pos;
					char *index;
					uint index_len;
					ulong num;
					zval **data;

					for (zend_hash_internal_pointer_reset_ex(parent_ht, &pos);
					     zend_hash_get_current_data_ex(parent_ht, (void**)&data, &pos) == SUCCESS &&
					     zend_hash_get_current_key_ex(parent_ht, &index, &index_len, &num, 0, &pos) == HASH_KEY_IS_STRING;
					     zend_hash_move_forward_ex(parent_ht, &pos)) {
						if (zend_hash_add(ht, index, index_len, data, sizeof(zval*), NULL) == SUCCESS) {
						    (*data)->refcount++;
						}
					}
				}
				zend_hash_update(directories, path, path_len+1, &ht, sizeof(HashTable*), NULL);
				ret = 1;
			}

			efree(name);
			efree(value);
		}

		if (ht == NULL) {
			ht = parent_ht;
		}

		if (keys) {
			DWORD i;
			char *name = (char*)emalloc(max_key+1);
			char *new_path = (char*)emalloc(path_len+max_key+2);
			DWORD name_len;
			FILETIME t;
			HKEY subkey;

			for (i = 0; i < keys; i++) {
				name_len = max_key+1;
				if (RegEnumKeyEx(key, i, name, &name_len, NULL, NULL, NULL, &t) == ERROR_SUCCESS) {
					if (RegOpenKeyEx(key, name, 0, KEY_READ, &subkey) == ERROR_SUCCESS) {
						if (path_len) {
							memcpy(new_path, path, path_len);
							new_path[path_len] = '/';
							path_len++;
						}
						memcpy(new_path+path_len, name, name_len+1);
						zend_str_tolower(new_path, path_len+name_len);
						if (LoadDirectory(directories, subkey, new_path, path_len+name_len, ht)) {
							ret = 1;
						}
						RegCloseKey(subkey);
					}
				}
			}
			efree(new_path);
			efree(name);
		}
	}
	return ret;
}

static void delete_internal_hashtable(void *data)
{
	zend_hash_destroy(*(HashTable**)data);
	free(*(HashTable**)data);
}

#define RegNotifyFlags (REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_ATTRIBUTES | REG_NOTIFY_CHANGE_LAST_SET)

void UpdateIniFromRegistry(char *path TSRMLS_DC)
{
	char *p, *orig_path;
	int path_len;
	HashTable **pht;

	if (!PW32G(registry_directories)) {
		PW32G(registry_directories) = (HashTable*)malloc(sizeof(HashTable));
		zend_hash_init(PW32G(registry_directories), 0, NULL, delete_internal_hashtable, 1);
		if (!OpenPhpRegistryKey("\\Per Directory Values", &PW32G(registry_key))) {
			PW32G(registry_key) = NULL;
			return;
		}
		PW32G(registry_event) = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (PW32G(registry_event)) {
			RegNotifyChangeKeyValue(PW32G(registry_key), TRUE, RegNotifyFlags, PW32G(registry_event), TRUE);
		}
		if (!LoadDirectory(PW32G(registry_directories), PW32G(registry_key), "", 0, NULL)) {
			return;
		}
	} else if (PW32G(registry_event) && WaitForSingleObject(PW32G(registry_event), 0) == WAIT_OBJECT_0) {
		RegNotifyChangeKeyValue(PW32G(registry_key), TRUE, RegNotifyFlags, PW32G(registry_event), TRUE);
		zend_hash_clean(PW32G(registry_directories));
		if (!LoadDirectory(PW32G(registry_directories), PW32G(registry_key), "", 0, NULL)) {
			return;
		}
	} else if (zend_hash_num_elements(PW32G(registry_directories)) == 0) {
		return;
	}

	orig_path = path = estrdup(path);

	/* Get rid of C:, if exists */
	p = strchr(path, ':');
	if (p) {
		*p = path[0];	/* replace the colon with the drive letter */
		path = p;		/* make path point to the drive letter */
	} else {
		if (path[0] != '\\' && path[0] != '/') {
			char tmp_buf[MAXPATHLEN], *cwd;
			char drive_letter;

			/* get current working directory and prepend it to the path */
			if (!VCWD_GETCWD(tmp_buf, MAXPATHLEN)) {
				efree(orig_path);
				return;
			}
			cwd = strchr(tmp_buf, ':');
			if (!cwd) {
				drive_letter = 'C';
				cwd = tmp_buf;
			} else {
				drive_letter = tmp_buf[0];
				cwd++;
			}
			while (*cwd == '\\' || *cwd == '/') {
				cwd++;
			}
			path = (char *) emalloc(2+strlen(cwd)+1+strlen(orig_path)+1);
			sprintf(path, "%c\\%s\\%s", drive_letter, cwd, orig_path);
			efree(orig_path);
			orig_path = path;
		}
	}

	path_len = 0;
	while (path[path_len] != 0) {
		if (path[path_len] == '\\') {
			path[path_len] = '/';
		}
		path_len++;
	}
	zend_str_tolower(path, path_len);
	while (path_len >= 0) {
		if (zend_hash_find(PW32G(registry_directories), path, path_len+1, (void**)&pht) == SUCCESS) {
			HashTable *ht = *pht;
			HashPosition pos;
			char *index;
			uint index_len;
			ulong num;
			zval **data;

			for (zend_hash_internal_pointer_reset_ex(ht, &pos);
			     zend_hash_get_current_data_ex(ht, (void**)&data, &pos) == SUCCESS &&
			     zend_hash_get_current_key_ex(ht, &index, &index_len, &num, 0, &pos) == HASH_KEY_IS_STRING;
			     zend_hash_move_forward_ex(ht, &pos)) {
				zend_alter_ini_entry(index, index_len, Z_STRVAL_PP(data), Z_STRLEN_PP(data), PHP_INI_SYSTEM, PHP_INI_STAGE_ACTIVATE);
			}
			break;
		}
		if (--path_len > 0) {
			while (path_len > 0 && path[path_len] != '/') {
				path_len--;
			}
		}
		path[path_len] = 0;
	}

	efree(orig_path);
}

#define PHPRC_REGISTRY_NAME "IniFilePath"

char *GetIniPathFromRegistry()
{
	char *reg_location = NULL;
	HKEY hKey;
	
	if (OpenPhpRegistryKey(NULL, &hKey)) {
		DWORD buflen = MAXPATHLEN;
		reg_location = emalloc(MAXPATHLEN+1);
		if(RegQueryValueEx(hKey, PHPRC_REGISTRY_NAME, 0, NULL, reg_location, &buflen) != ERROR_SUCCESS) {
			efree(reg_location);
			reg_location = NULL;
			return reg_location;
		}
		RegCloseKey(hKey);
	}
	return reg_location;
}