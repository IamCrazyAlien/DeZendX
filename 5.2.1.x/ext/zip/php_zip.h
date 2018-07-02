/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Pierre-Alain Joye <pajoye@php.net>                           |
  +----------------------------------------------------------------------+
*/

/* $Id: php_zip.h,v 1.10.2.2 2007/01/01 09:36:10 sebastian Exp $ */

#ifndef PHP_ZIP_H
#define PHP_ZIP_H

extern zend_module_entry zip_module_entry;
#define phpext_zip_ptr &zip_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

#include "lib/zip.h"

typedef struct _ze_zip_rsrc {
	struct zip *za;
	int index_current;
	int num_files;
} zip_rsrc;

typedef zip_rsrc * zip_rsrc_ptr;

typedef struct _ze_zip_read_rsrc {
	struct zip_file *zf;
	struct zip_stat sb;
} zip_read_rsrc;

#define ZIPARCHIVE_ME(name, arg_info, flags)	ZEND_FENTRY(name, c_ziparchive_ ##name, arg_info, flags)
#define ZIPARCHIVE_METHOD(name)	ZEND_NAMED_FUNCTION(c_ziparchive_##name)

/* Extends zend object */
typedef struct _ze_zip_object {
	zend_object zo;
	struct zip *za;
	int buffers_cnt;
	char **buffers;
	HashTable *prop_handler;
	char *filename;
	int filename_len;
} ze_zip_object;

php_stream *php_stream_zip_opener(php_stream_wrapper *wrapper, char *path, char *mode, int options, char **opened_path, php_stream_context *context STREAMS_DC TSRMLS_DC);
php_stream *php_stream_zip_open(char *filename, char *path, char *mode STREAMS_DC TSRMLS_DC);

extern php_stream_wrapper php_stream_zip_wrapper;

#endif	/* PHP_ZIP_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
