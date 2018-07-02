/*
**      msql.h  -
**
**
** Copyright (c) 1993-95  David J. Hughes
** Copyright (c) 1995  Hughes Technologies Pty Ltd
**
** Permission to use, copy, and distribute for non-commercial purposes,
** is hereby granted without fee, providing that the above copyright
** notice appear in all copies and that both the copyright notice and this
** permission notice appear in supporting documentation.
**
** This software is provided "as is" without any expressed or implied warranty.
**
** ID = "$Id:"
**
*/


#ifndef MSQL_H
#define MSQL_H

#ifndef APIENTRY
#  if defined(OS2)
#    ifdef BCPP
#      define _System   _syscall
#      define _Optlink
#    endif
#    define APIENTRY _System
#  elif defined(WIN32)
#    define APIENTRY __stdcall
#  else
#    define APIENTRY
#  endif
#endif

#if defined (WIN32) || defined(__STDC__) || defined(__cplusplus)
#  define __ANSI_PROTO(x)       x
#else
#  define __ANSI_PROTO(x)       ()
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef char    ** m_row;

typedef struct field_s {
        char    *name,
                *table;
        int     type,
                length,
                flags;
} m_field;


typedef struct  m_seq_s {
        int     step,
                value;
} m_seq;


typedef struct  m_data_s {
        int     width;
        m_row   data;
        struct  m_data_s *next;
} m_data;

typedef struct m_fdata_s {
        m_field field;
        struct m_fdata_s *next;
} m_fdata;



typedef struct result_s {
        m_data  *queryData,
                *cursor;
        m_fdata *fieldData,
                *fieldCursor;
        int     numRows,
                numFields;
} m_result;


#define msqlNumRows(res) res->numRows
#define msqlNumFields(res) res->numFields


#define INT_TYPE        1
#define CHAR_TYPE       2
#define REAL_TYPE       3
#define IDENT_TYPE      4
#define NULL_TYPE       5
#define TEXT_TYPE       6
#define DATE_TYPE       7
#define UINT_TYPE       8
#define MONEY_TYPE      9
#define TIME_TYPE       10
#define LAST_REAL_TYPE  10
#define IDX_TYPE        253
#define SYSVAR_TYPE     254
#define ANY_TYPE        255

#define NOT_NULL_FLAG   1
#define UNIQUE_FLAG     2

#define IS_UNIQUE(n)    (n & UNIQUE_FLAG)
#define IS_NOT_NULL(n)  (n & NOT_NULL_FLAG)

static char msqlTypeNames[][12] =
        {"???", "int", "char","real","ident","null","text","date","uint",
        "money","time","???"};


/*
** Pre-declarations for the API library functions
*/
#ifndef _MSQL_SERVER_SOURCE
#  ifndef _LIB_SOURCE
#    if defined(OS2) || defined(WIN32)
#       define msqlErrMsg   msqlGetErrMsg(NULL)
#    else
        extern char         msqlErrMsg[];
#    endif
#  endif
        char     * APIENTRY msqlGetErrMsg __ANSI_PROTO((char *));    /* OS/2 */
        int        APIENTRY msqlUserConnect __ANSI_PROTO((char *, char *)); /* OS/2 */
        int        APIENTRY msqlConnect __ANSI_PROTO((char *));
        int        APIENTRY msqlSelectDB __ANSI_PROTO((int, char*));
        int        APIENTRY msqlQuery __ANSI_PROTO((int, char*));
        int        APIENTRY msqlCreateDB __ANSI_PROTO((int, char*));
        int        APIENTRY msqlCopyDB __ANSI_PROTO((int, char*, char*)); /* OS/2 */
        int        APIENTRY msqlMoveDB __ANSI_PROTO((int, char*, char*)); /* OS/2 */
        int        APIENTRY msqlDropDB __ANSI_PROTO((int, char*));
        int        APIENTRY msqlShutdown __ANSI_PROTO((int));
        int        APIENTRY msqlGetProtoInfo();
        int        APIENTRY msqlReloadAcls __ANSI_PROTO((int));
        int        APIENTRY msqlGetServerStats __ANSI_PROTO((int));
        char     * APIENTRY msqlGetServerInfo();
        char     * APIENTRY msqlGetHostInfo();
        char     * APIENTRY msqlUnixTimeToDate __ANSI_PROTO((time_t));
        char     * APIENTRY msqlUnixTimeToTime __ANSI_PROTO((time_t));
        void       APIENTRY msqlClose __ANSI_PROTO((int));
        void       APIENTRY msqlDataSeek __ANSI_PROTO((m_result*, int));
        void       APIENTRY msqlFieldSeek __ANSI_PROTO((m_result*, int));
        void       APIENTRY msqlFreeResult __ANSI_PROTO((m_result*));
        m_row      APIENTRY msqlFetchRow __ANSI_PROTO((m_result*));
        m_seq    * APIENTRY msqlGetSequenceInfo __ANSI_PROTO((int, char*));
        m_field  * APIENTRY msqlFetchField __ANSI_PROTO((m_result *));
        m_result * APIENTRY msqlListDBs __ANSI_PROTO((int));
        m_result * APIENTRY msqlListTables __ANSI_PROTO((int));
        m_result * APIENTRY msqlListFields __ANSI_PROTO((int, char*));
        m_result * APIENTRY msqlListIndex __ANSI_PROTO((int, char*, char*));
        m_result * APIENTRY msqlStoreResult __ANSI_PROTO((void));
        time_t     APIENTRY msqlDateToUnixTime __ANSI_PROTO((char *));
        time_t     APIENTRY msqlTimeToUnixTime __ANSI_PROTO((char *));
        char     * APIENTRY msqlSumTimes __ANSI_PROTO((char *, char *));      /* OS/2 !!! */
        char     * APIENTRY msqlDateOffset __ANSI_PROTO((char *, int, int, int)); /* OS/2 !!! */
        char     * APIENTRY msqlDiffTimes __ANSI_PROTO((char *, char *));     /* OS/2 !!! */
        int        APIENTRY msqlDiffDates __ANSI_PROTO((char *, char *));     /* OS/2 !!! */
#endif

int                APIENTRY msqlLoadConfigFile __ANSI_PROTO((char *));
int                APIENTRY msqlGetIntConf __ANSI_PROTO((char *));
char             * APIENTRY msqlGetCharConf __ANSI_PROTO((char *));


#ifdef __cplusplus
        }
#endif
#endif /*MSQL_H*/
