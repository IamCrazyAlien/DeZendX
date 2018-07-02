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
   | Author: Jaakko Hyv�tti <jaakko.hyvatti@iki.fi>                       |
   +----------------------------------------------------------------------+
 */

/* $Id: php_sprintf.c,v 1.23.2.1.2.1 2007/01/01 09:36:11 sebastian Exp $ */

#include <stdio.h>
#include <stdarg.h>
#include "php.h"
#ifdef PHP_WIN32
#include "config.w32.h"
#else
#include <php_config.h>
#endif

PHPAPI int
php_sprintf (char*s, const char* format, ...)
{
  va_list args;
  int ret;

  va_start (args, format);
  s[0] = '\0';
  ret = vsprintf (s, format, args);
  va_end (args);
  if (!ret)
    return -1;
  return strlen (s);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
