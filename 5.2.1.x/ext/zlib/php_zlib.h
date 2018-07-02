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
   | Authors: Rasmus Lerdorf <rasmus@lerdorf.on.ca>                       |
   |          Stefan R�hrich <sr@linux.de>                                |
   +----------------------------------------------------------------------+
*/

/* $Id: php_zlib.h,v 1.42.2.1.2.2 2007/01/01 09:36:10 sebastian Exp $ */

#ifndef PHP_ZLIB_H
#define PHP_ZLIB_H

#include <zlib.h>

ZEND_BEGIN_MODULE_GLOBALS(zlib)
	/* variables for transparent gzip encoding */
	int compression_coding;
	z_stream stream;
	uLong crc;
	int ob_gzhandler_status;
	long output_compression;
	long output_compression_level;
	char *output_handler;
ZEND_END_MODULE_GLOBALS(zlib)

extern php_stream_filter_factory php_zlib_filter_factory;
extern zend_module_entry php_zlib_module_entry;
#define zlib_module_ptr &php_zlib_module_entry

int php_ob_gzhandler_check(TSRMLS_D);

php_stream *php_stream_gzopen(php_stream_wrapper *wrapper, char *path, char *mode, int options, char **opened_path, php_stream_context *context STREAMS_DC TSRMLS_DC);
extern php_stream_wrapper php_stream_gzip_wrapper;

#ifdef ZTS
#define ZLIBG(v) TSRMG(zlib_globals_id, zend_zlib_globals *, v)
#else
#define ZLIBG(v) (zlib_globals.v)
#endif

#define phpext_zlib_ptr zlib_module_ptr

#define CODING_GZIP		1
#define CODING_DEFLATE	2

#endif /* PHP_ZLIB_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
