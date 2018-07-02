/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2007 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        | 
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Authors: Wez Furlong <wez@thebrainroom.com>                          |
   +----------------------------------------------------------------------+
*/

/* $Id: zend_stream.c,v 1.13.2.1.2.1 2007/01/01 09:35:47 sebastian Exp $ */


#include "zend.h"
#include "zend_compile.h"

ZEND_DLIMPORT int isatty(int fd);

static size_t zend_stream_stdio_reader(void *handle, char *buf, size_t len TSRMLS_DC)
{
	return fread(buf, 1, len, (FILE*)handle);
}

static void zend_stream_stdio_closer(void *handle TSRMLS_DC)
{
	if ((FILE*)handle != stdin)
		fclose((FILE*)handle);
}

static long zend_stream_stdio_fteller(void *handle TSRMLS_DC)
{
	return ftell((FILE*) handle);
}


ZEND_API int zend_stream_open(const char *filename, zend_file_handle *handle TSRMLS_DC)
{
	if (zend_stream_open_function) {
		return zend_stream_open_function(filename, handle TSRMLS_CC);
	}
	handle->type = ZEND_HANDLE_FP;
	handle->opened_path = NULL;
	handle->handle.fp = zend_fopen(filename, &handle->opened_path);
	handle->filename = (char *)filename;
	handle->free_filename = 0;
	
	return (handle->handle.fp) ? SUCCESS : FAILURE;
}

ZEND_API int zend_stream_fixup(zend_file_handle *file_handle TSRMLS_DC)
{
	switch (file_handle->type) {
		case ZEND_HANDLE_FILENAME:
			if (FAILURE == zend_stream_open(file_handle->filename, file_handle TSRMLS_CC)) {
				return FAILURE;
			}
			break;
			
		case ZEND_HANDLE_FD:
			file_handle->handle.fp = fdopen(file_handle->handle.fd, "rb");
			file_handle->type = ZEND_HANDLE_FP;
			break;
			
		case ZEND_HANDLE_FP:
			file_handle->handle.fp = file_handle->handle.fp;
			break;
			
		case ZEND_HANDLE_STREAM:
			/* nothing to do */
			return SUCCESS;
			
		default:
			return FAILURE;
	}
	if (file_handle->type == ZEND_HANDLE_FP) {
		if (!file_handle->handle.fp) {
			return FAILURE;
		}

		/* make compatible with stream */
		file_handle->handle.stream.handle = file_handle->handle.fp;
		file_handle->handle.stream.reader = zend_stream_stdio_reader;
		file_handle->handle.stream.closer = zend_stream_stdio_closer;
		file_handle->handle.stream.fteller = zend_stream_stdio_fteller;

		file_handle->handle.stream.interactive = isatty(fileno((FILE *)file_handle->handle.stream.handle));
	}
	return SUCCESS;
}

ZEND_API size_t zend_stream_read(zend_file_handle *file_handle, char *buf, size_t len TSRMLS_DC)
{
	if (file_handle->handle.stream.interactive) {
		int c = '*';
		size_t n; 

#ifdef NETWARE
		/*
			c != 4 check is there as fread of a character in NetWare LibC gives 4 upon ^D character.
			Ascii value 4 is actually EOT character which is not defined anywhere in the LibC
			or else we can use instead of hardcoded 4.
		*/
                for ( n = 0; n < len && (c = zend_stream_getc( file_handle TSRMLS_CC)) != EOF && c != 4 && c != '\n'; ++n )
#else
		for ( n = 0; n < len && (c = zend_stream_getc( file_handle TSRMLS_CC)) != EOF && c != '\n'; ++n ) 
#endif
			buf[n] = (char) c; 
		if ( c == '\n' )
			buf[n++] = (char) c; 

		return n;
	}
	return file_handle->handle.stream.reader(file_handle->handle.stream.handle, buf, len TSRMLS_CC);
}

ZEND_API int zend_stream_getc(zend_file_handle *file_handle TSRMLS_DC)
{
	char buf;

	if (file_handle->handle.stream.reader(file_handle->handle.stream.handle, &buf, sizeof(buf) TSRMLS_CC)) {
		return (int)buf;
	}
	return EOF;
}

ZEND_API int zend_stream_ferror(zend_file_handle *file_handle TSRMLS_DC)
{
	return 0;
}

ZEND_API long zend_stream_ftell(zend_file_handle *file_handle TSRMLS_DC)
{
	return file_handle->handle.stream.fteller(file_handle->handle.stream.handle TSRMLS_CC);
}
