/* 
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2007 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Jani Lehtim�ki <jkl@njet.net>                                |
   +----------------------------------------------------------------------+
*/

/* $Id: php_var.h,v 1.30.2.1.2.5 2007/01/01 09:36:08 sebastian Exp $ */

#ifndef PHP_VAR_H
#define PHP_VAR_H

#include "ext/standard/php_smart_str_public.h"

PHP_FUNCTION(var_dump);
PHP_FUNCTION(var_export);
PHP_FUNCTION(debug_zval_dump);
PHP_FUNCTION(serialize);
PHP_FUNCTION(unserialize);
PHP_FUNCTION(memory_get_usage);
PHP_FUNCTION(memory_get_peak_usage);

PHPAPI void php_var_dump(zval **struc, int level TSRMLS_DC);
PHPAPI void php_var_export(zval **struc, int level TSRMLS_DC);
PHPAPI void php_debug_zval_dump(zval **struc, int level TSRMLS_DC);

/* typdef HashTable php_serialize_data_t; */
#define php_serialize_data_t HashTable

struct php_unserialize_data {
	void *first;
	void *first_dtor;
};

typedef struct php_unserialize_data php_unserialize_data_t;

PHPAPI void php_var_serialize(smart_str *buf, zval **struc, php_serialize_data_t *var_hash TSRMLS_DC);
PHPAPI int php_var_unserialize(zval **rval, const unsigned char **p, const unsigned char *max, php_unserialize_data_t *var_hash TSRMLS_DC);

#define PHP_VAR_SERIALIZE_INIT(var_hash) \
   zend_hash_init(&(var_hash), 10, NULL, NULL, 0)
#define PHP_VAR_SERIALIZE_DESTROY(var_hash) \
   zend_hash_destroy(&(var_hash))

#define PHP_VAR_UNSERIALIZE_INIT(var_hash) \
	(var_hash).first = 0; \
	(var_hash).first_dtor = 0
#define PHP_VAR_UNSERIALIZE_DESTROY(var_hash) \
	var_destroy(&(var_hash))

PHPAPI void var_replace(php_unserialize_data_t *var_hash, zval *ozval, zval **nzval);
PHPAPI void var_destroy(php_unserialize_data_t *var_hash);

#define PHP_VAR_UNSERIALIZE_ZVAL_CHANGED(var_hash, ozval, nzval) \
	var_replace((var_hash), (ozval), &(nzval))
	
PHPAPI zend_class_entry *php_create_empty_class(char *class_name, int len);

#endif /* PHP_VAR_H */
