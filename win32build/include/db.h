/* DO NOT EDIT: automatically built by dist/distrib. */
/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1996, 1997, 1998
 *	Sleepycat Software.  All rights reserved.
 *
 *	@(#)db.h.src	10.131 (Sleepycat) 6/2/98
 */

#ifndef _DB_H_
#define	_DB_H_

#ifndef __NO_SYSTEM_INCLUDES
#include <sys/types.h>

#include <stdio.h>
#endif

/*
 * XXX
 * MacOS: ensure that Metrowerks C makes enumeration types int sized.
 */
#ifdef __MWERKS__
#pragma enumsalwaysint on
#endif

/*
 * XXX
 * Handle function prototypes and the keyword "const".  This steps on name
 * space that DB doesn't control, but all of the other solutions are worse.
 *
 * XXX
 * While Microsoft's compiler is ANSI C compliant, it doesn't have _STDC_
 * defined by default, you specify a command line flag or #pragma to turn
 * it on.  Don't do that, however, because some of Microsoft's own header
 * files won't compile.
 */
#undef	__P
#if defined(__STDC__) || defined(__cplusplus) || defined(_MSC_VER)
#define	__P(protos)	protos		/* ANSI C prototypes */
#else
#define	const
#define	__P(protos)	()		/* K&R C preprocessor */
#endif

/*
 * !!!
 * DB needs basic information about specifically sized types.  If they're
 * not provided by the system, typedef them here.
 *
 * We protect them against multiple inclusion using __BIT_TYPES_DEFINED__,
 * as does BIND and Kerberos, since we don't know for sure what #include
 * files the user is using.
 *
 * !!!
 * We also provide the standard u_int, u_long etc., if they're not provided
 * by the system.
 */
#ifndef	__BIT_TYPES_DEFINED__
#define	__BIT_TYPES_DEFINED__
typedef unsigned char u_int8_t;
typedef short int16_t;
typedef unsigned short u_int16_t;
typedef int int32_t;
typedef unsigned int u_int32_t;
#endif

#if !defined(_WINSOCKAPI_)
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
#endif

#define	DB_VERSION_MAJOR	2
#define	DB_VERSION_MINOR	4
#define	DB_VERSION_PATCH	14
#define	DB_VERSION_STRING	"Sleepycat Software: DB 2.4.14: (6/2/98)"

typedef	u_int32_t	db_pgno_t;	/* Page number type. */
typedef	u_int16_t	db_indx_t;	/* Page offset type. */
#define	DB_MAX_PAGES	0xffffffff	/* >= # of pages in a file */

typedef	u_int32_t	db_recno_t;	/* Record number type. */
typedef size_t		DB_LOCK;	/* Object returned by lock manager. */
#define	DB_MAX_RECORDS	0xffffffff	/* >= # of records in a tree */

#define	DB_FILE_ID_LEN		20	/* DB file ID length. */

/* Forward structure declarations, so applications get type checking. */
struct __db;		typedef struct __db DB;
#ifdef DB_DBM_HSEARCH
			typedef struct __db DBM;
#endif
struct __db_bt_stat;	typedef struct __db_bt_stat DB_BTREE_STAT;
struct __db_dbt;	typedef struct __db_dbt DBT;
struct __db_env;	typedef struct __db_env DB_ENV;
struct __db_info;	typedef struct __db_info DB_INFO;
struct __db_lock_stat;	typedef struct __db_lock_stat DB_LOCK_STAT;
struct __db_lockregion;	typedef struct __db_lockregion DB_LOCKREGION;
struct __db_lockreq;	typedef struct __db_lockreq DB_LOCKREQ;
struct __db_locktab;	typedef struct __db_locktab DB_LOCKTAB;
struct __db_log;	typedef struct __db_log DB_LOG;
struct __db_log_stat;	typedef struct __db_log_stat DB_LOG_STAT;
struct __db_lsn;	typedef struct __db_lsn DB_LSN;
struct __db_mpool;	typedef struct __db_mpool DB_MPOOL;
struct __db_mpool_finfo;typedef struct __db_mpool_finfo DB_MPOOL_FINFO;
struct __db_mpool_fstat;typedef struct __db_mpool_fstat DB_MPOOL_FSTAT;
struct __db_mpool_stat;	typedef struct __db_mpool_stat DB_MPOOL_STAT;
struct __db_mpoolfile;	typedef struct __db_mpoolfile DB_MPOOLFILE;
struct __db_txn;	typedef struct __db_txn DB_TXN;
struct __db_txn_active;	typedef struct __db_txn_active DB_TXN_ACTIVE;
struct __db_txn_stat;	typedef struct __db_txn_stat DB_TXN_STAT;
struct __db_txnmgr;	typedef struct __db_txnmgr DB_TXNMGR;
struct __db_txnregion;	typedef struct __db_txnregion DB_TXNREGION;
struct __dbc;		typedef struct __dbc DBC;

/* Key/data structure -- a Data-Base Thang. */
struct __db_dbt {
	void	 *data;			/* key/data */
	u_int32_t size;			/* key/data length */
	u_int32_t ulen;			/* RO: length of user buffer. */
	u_int32_t dlen;			/* RO: get/put record length. */
	u_int32_t doff;			/* RO: get/put record offset. */

#define	DB_DBT_INTERNAL	0x01		/* Perform any mallocs using regular
					   malloc, not the user's malloc. */
#define	DB_DBT_MALLOC	0x02		/* Return in allocated memory. */
#define	DB_DBT_PARTIAL	0x04		/* Partial put/get. */
#define	DB_DBT_USERMEM	0x08		/* Return in user's memory. */
	u_int32_t flags;
};

/*
 * DB internal configuration.
 *
 * There are a set of functions that the application can replace with its
 * own versions, and some other knobs which can be turned at run-time.
 */
#define	DB_FUNC_CALLOC	 1	/* DELETED: ANSI C calloc. */
#define	DB_FUNC_CLOSE	 2		/* POSIX 1003.1 close. */
#define	DB_FUNC_DIRFREE	 3		/* DB: free directory list. */
#define	DB_FUNC_DIRLIST	 4		/* DB: create directory list. */
#define	DB_FUNC_EXISTS	 5		/* DB: return if file exists. */
#define	DB_FUNC_FREE	 6		/* ANSI C free. */
#define	DB_FUNC_FSYNC	 7		/* POSIX 1003.1 fsync. */
#define	DB_FUNC_IOINFO	 8		/* DB: return file I/O information. */
#define	DB_FUNC_MALLOC	 9		/* ANSI C malloc. */
#define	DB_FUNC_MAP	10		/* DB: map file into shared memory. */
#define	DB_FUNC_OPEN	11		/* POSIX 1003.1 open. */
#define	DB_FUNC_READ	12		/* POSIX 1003.1 read. */
#define	DB_FUNC_REALLOC	13		/* ANSI C realloc. */
#define	DB_FUNC_SEEK	14		/* POSIX 1003.1 lseek. */
#define	DB_FUNC_SLEEP	15		/* DB: sleep secs/usecs. */
#define	DB_FUNC_STRDUP	16	/* DELETED: DB: strdup(3). */
#define	DB_FUNC_UNLINK	17		/* POSIX 1003.1 unlink. */
#define	DB_FUNC_UNMAP	18		/* DB: unmap shared memory file. */
#define	DB_FUNC_WRITE	19		/* POSIX 1003.1 write. */
#define	DB_FUNC_YIELD	20		/* DB: yield thread to scheduler. */
#define	DB_TSL_SPINS	21		/* DB: initialize spin count. */
#define	DB_FUNC_RUNLINK	22		/* DB: remove a shared region. */
#define	DB_REGION_ANON	23		/* DB: anonymous, unnamed regions. */
#define	DB_REGION_INIT	24		/* DB: page-fault regions in create. */
#define	DB_REGION_NAME	25		/* DB: anonymous, named regions. */
#define	DB_MUTEXLOCKS	26		/* DB: turn off all mutex locks. */
#define	DB_PAGEYIELD	27		/* DB: yield the CPU on pool get. */

/*
 * Database configuration and initialization.
 */
 /*
  * Flags understood by both db_open(3) and db_appinit(3).
  */
#define	DB_CREATE	      0x000001	/* O_CREAT: create file as necessary. */
#define	DB_NOMMAP	      0x000002	/* Don't mmap underlying file. */
#define	DB_THREAD	      0x000004	/* Free-thread DB package handles. */

/*
 * Flags understood by db_appinit(3).
 */
/*			      0x000007	   COMMON MASK. */
#define	DB_INIT_LOCK	      0x000008	/* Initialize locking. */
#define	DB_INIT_LOG	      0x000010	/* Initialize logging. */
#define	DB_INIT_MPOOL	      0x000020	/* Initialize mpool. */
#define	DB_INIT_TXN	      0x000040	/* Initialize transactions. */
#define	DB_MPOOL_PRIVATE      0x000080	/* Mpool: private memory pool. */
#define	__UNUSED_100	      0x000100
#define	DB_RECOVER	      0x000200	/* Run normal recovery. */
#define	DB_RECOVER_FATAL      0x000400	/* Run catastrophic recovery. */
#define	DB_TXN_NOSYNC	      0x000800	/* Do not sync log on commit. */
#define	DB_USE_ENVIRON	      0x001000	/* Use the environment. */
#define	DB_USE_ENVIRON_ROOT   0x002000	/* Use the environment if root. */

/* CURRENTLY UNUSED LOCK FLAGS. */
#define	DB_TXN_LOCK_2PL	      0x000000	/* Two-phase locking. */
#define	DB_TXN_LOCK_OPTIMIST  0x000000	/* Optimistic locking. */
#define	DB_TXN_LOCK_MASK      0x000000	/* Lock flags mask. */

/* CURRENTLY UNUSED LOG FLAGS. */
#define	DB_TXN_LOG_REDO	      0x000000	/* Redo-only logging. */
#define	DB_TXN_LOG_UNDO	      0x000000	/* Undo-only logging. */
#define	DB_TXN_LOG_UNDOREDO   0x000000	/* Undo/redo write-ahead logging. */
#define	DB_TXN_LOG_MASK	      0x000000	/* Log flags mask. */

/*
 * Flags understood by db_open(3).
 *
 * DB_EXCL and DB_TEMPORARY are internal only, and are not documented.
 * DB_SEQUENTIAL is currently internal, but may be exported some day.
 */
/*			      0x000007	   COMMON MASK. */
/*			      0x003fff	   ALREADY USED. */
#define	__UNUSED_4000	      0x004000
#define	DB_EXCL		      0x008000	/* O_EXCL: exclusive open. */
#define	DB_RDONLY	      0x010000	/* O_RDONLY: read-only. */
#define	DB_SEQUENTIAL	      0x020000	/* Indicate sequential access. */
#define	DB_TEMPORARY	      0x040000	/* Remove on last close. */
#define	DB_TRUNCATE	      0x080000	/* O_TRUNCATE: replace existing DB. */

/*
 * Deadlock detector modes; used in the DBENV structure to configure the
 * locking subsystem.
 */
#define	DB_LOCK_NORUN		0x0
#define	DB_LOCK_DEFAULT		0x1	/* Default policy. */
#define	DB_LOCK_OLDEST		0x2	/* Abort oldest transaction. */
#define	DB_LOCK_RANDOM		0x3	/* Abort random transaction. */
#define	DB_LOCK_YOUNGEST	0x4	/* Abort youngest transaction. */

struct __db_env {
	int		 db_lorder;	/* Byte order. */

					/* Error message callback. */
	void (*db_errcall) __P((const char *, char *));
	FILE		*db_errfile;	/* Error message file stream. */
	const char	*db_errpfx;	/* Error message prefix. */
	int		 db_verbose;	/* Generate debugging messages. */

	/* User paths. */
	char		*db_home;	/* Database home. */
	char		*db_log_dir;	/* Database log file directory. */
	char		*db_tmp_dir;	/* Database tmp file directory. */

	char	       **db_data_dir;	/* Database data file directories. */
	int		 data_cnt;	/* Database data file slots. */
	int		 data_next;	/* Next Database data file slot. */

	/* Locking. */
	DB_LOCKTAB	*lk_info;	/* Return from lock_open(). */
	u_int8_t	*lk_conflicts;	/* Two dimensional conflict matrix. */
	u_int32_t	 lk_modes;	/* Number of lock modes in table. */
	u_int32_t	 lk_max;	/* Maximum number of locks. */
	u_int32_t	 lk_detect;	/* Deadlock detect on all conflicts. */

	/* Logging. */
	DB_LOG		*lg_info;	/* Return from log_open(). */
	u_int32_t	 lg_max;	/* Maximum file size. */

	/* Memory pool. */
	DB_MPOOL	*mp_info;	/* Return from memp_open(). */
	size_t		 mp_mmapsize;	/* Maximum file size for mmap. */
	size_t		 mp_size;	/* Bytes in the mpool cache. */

	/* Transactions. */
	DB_TXNMGR	*tx_info;	/* Return from txn_open(). */
	u_int32_t	 tx_max;	/* Maximum number of transactions. */
	int (*tx_recover)		/* Dispatch function for recovery. */
	    __P((DB_LOG *, DBT *, DB_LSN *, int, void *));

#define	DB_ENV_APPINIT		0x01	/* Paths initialized by db_appinit(). */
#define	DB_ENV_STANDALONE	0x02	/* Test: freestanding environment. */
#define	DB_ENV_THREAD		0x04	/* DB_ENV is multi-threaded. */
	u_int32_t	 flags;		/* Flags. */
};

/*******************************************************
 * Access methods.
 *******************************************************/
/*
 * XXX
 * Changes here must be reflected in java/src/com/sleepycat/db/Db.java.
 */
typedef enum {
	DB_BTREE=1,			/* B+tree. */
	DB_HASH,			/* Extended Linear Hashing. */
	DB_RECNO,			/* Fixed and variable-length records. */
	DB_UNKNOWN			/* Figure it out on open. */
} DBTYPE;

#define	DB_BTREEVERSION	6		/* Current btree version. */
#define	DB_BTREEOLDVER	6		/* Oldest btree version supported. */
#define	DB_BTREEMAGIC	0x053162

#define	DB_HASHVERSION	5		/* Current hash version. */
#define	DB_HASHOLDVER	4		/* Oldest hash version supported. */
#define	DB_HASHMAGIC	0x061561

#define	DB_LOGVERSION	2		/* Current log version. */
#define	DB_LOGOLDVER	2		/* Oldest log version supported. */
#define	DB_LOGMAGIC	0x040988

struct __db_info {
	int		 db_lorder;	/* Byte order. */
	size_t		 db_cachesize;	/* Underlying cache size. */
	size_t		 db_pagesize;	/* Underlying page size. */

					/* Local heap allocation. */
	void *(*db_malloc) __P((size_t));

	/* Btree access method. */
	u_int32_t	 bt_maxkey;	/* Maximum keys per page. */
	u_int32_t	 bt_minkey;	/* Minimum keys per page. */
	int (*bt_compare)		/* Comparison function. */
	    __P((const DBT *, const DBT *));
	size_t (*bt_prefix)		/* Prefix function. */
	    __P((const DBT *, const DBT *));

	/* Hash access method. */
	u_int32_t 	 h_ffactor;	/* Fill factor. */
	u_int32_t	 h_nelem;	/* Number of elements. */
	u_int32_t      (*h_hash)	/* Hash function. */
	    __P((const void *, u_int32_t));

	/* Recno access method. */
	int		 re_pad;	/* Fixed-length padding byte. */
	int		 re_delim;	/* Variable-length delimiting byte. */
	u_int32_t	 re_len;	/* Length for fixed-length records. */
	char		*re_source;	/* Source file name. */

#define	DB_DELIMITER		0x0001	/* Recno: re_delim set. */
#define	DB_DUP			0x0002	/* Btree, Hash: duplicate keys. */
#define	DB_FIXEDLEN		0x0004	/* Recno: fixed-length records. */
#define	DB_PAD			0x0008	/* Recno: re_pad set. */
#define	DB_RECNUM		0x0010	/* Btree: record numbers. */
#define	DB_RENUMBER		0x0020	/* Recno: renumber on insert/delete. */
#define	DB_SNAPSHOT		0x0040	/* Recno: snapshot the input. */
	u_int32_t	 flags;
};

/*
 * DB access method and cursor operation codes.  These are implemented as
 * bit fields for future flexibility, but currently only a single one may
 * be specified to any function.
 */
#define	DB_AFTER	0x000001	/* c_put() */
#define	DB_APPEND	0x000002	/* put() */
#define	DB_BEFORE	0x000004	/* c_put() */
#define	DB_CHECKPOINT	0x000008	/* log_put(), log_get() */
#define	DB_CURRENT	0x000010	/* c_get(), c_put(), log_get() */
#define	DB_FIRST	0x000020	/* c_get(), log_get() */
#define	DB_FLUSH	0x000040	/* log_put() */
#define	DB_GET_RECNO	0x000080	/* get(), c_get() */
#define	DB_KEYFIRST	0x000100	/* c_put() */
#define	DB_KEYLAST	0x000200	/* c_put() */
#define	DB_LAST		0x000400	/* c_get(), log_get() */
#define	DB_NEXT		0x000800	/* c_get(), log_get() */
#define	DB_NOOVERWRITE	0x001000	/* put() */
#define	DB_NOSYNC	0x002000	/* close() */
#define	DB_PREV		0x004000	/* c_get(), log_get() */
#define	DB_RECORDCOUNT	0x008000	/* stat() */
#define	DB_SET		0x010000	/* c_get(), log_get() */
#define	DB_SET_RANGE	0x020000	/* c_get() */
#define	DB_SET_RECNO	0x040000	/* c_get() */
#define	DB_CURLSN	0x080000	/* log_put() */

/*
 * DB (user visible) error return codes.
 *
 * XXX
 * Changes to any of the user visible error return codes must be reflected
 * in java/src/com/sleepycat/db/Db.java.
 */
#define	DB_INCOMPLETE		( -1)	/* Sync didn't finish. */
#define	DB_KEYEMPTY		( -2)	/* The key/data pair was deleted or
					   was never created by the user. */
#define	DB_KEYEXIST		( -3)	/* The key/data pair already exists. */
#define	DB_LOCK_DEADLOCK	( -4)	/* Locker killed to resolve deadlock. */
#define	DB_LOCK_NOTGRANTED	( -5)	/* Lock unavailable, no-wait set. */
#define	DB_LOCK_NOTHELD		( -6)	/* Lock not held by locker. */
#define	DB_NOTFOUND		( -7)	/* Key/data pair not found (EOF). */

/* DB (private) error return codes. */
#define	DB_DELETED		( -8)	/* Recovery file marked deleted. */
#define	DB_NEEDSPLIT		( -9)	/* Page needs to be split. */
#define	DB_REGISTERED		(-10)	/* Entry was previously registered. */
#define	DB_SWAPBYTES		(-11)	/* Database needs byte swapping. */
#define DB_TXN_CKP		(-12)	/* Encountered ckp record in log. */

struct __db_ilock {			/* Internal DB access method lock. */
	db_pgno_t	pgno;		/* Page being locked. */
					/* File id. */
	u_int8_t	fileid[DB_FILE_ID_LEN];
};

/* DB access method description structure. */
struct __db {
	void	*mutexp;		/* Synchronization for free threading */
	DBTYPE	 type;			/* DB access method. */
	DB_ENV	*dbenv;			/* DB_ENV structure. */
	DB_ENV	*mp_dbenv;		/* DB_ENV for local mpool creation. */

	DB	*master;		/* Original DB created by db_open. */
	void	*internal;		/* Access method private. */

	DB_MPOOL	*mp;		/* The access method's mpool. */
	DB_MPOOLFILE	*mpf;		/* The access method's mpool file. */

	/*
	 * XXX
	 * Explicit representations of structures in queue.h.
	 *
	 * TAILQ_HEAD(curs_queue, __dbc);
	 */
	struct {
		struct __dbc *tqh_first;
		struct __dbc **tqh_last;
	} curs_queue;

	/*
	 * XXX
	 * Explicit representations of structures in queue.h.
	 *
	 * LIST_HEAD(handleq, __db);
	 * LIST_ENTRY(__db);
	 */
	struct {
		struct __db *lh_first;
	} handleq;			/* List of handles for this DB. */
	struct {
		struct __db *le_next;
		struct __db **le_prev;
	} links;			/* Links for the handle list. */

	u_int32_t log_fileid;		/* Logging file id. */

	DB_TXN	 *txn;			/* Current transaction. */
	u_int32_t locker;		/* Default process' locker id. */
	DBT	  lock_dbt;		/* DBT referencing lock. */
	struct __db_ilock lock;		/* Lock. */

	size_t	  pgsize;		/* Logical page size of file. */

					/* Local heap allocation. */
	void *(*db_malloc) __P((size_t));

					/* Functions. */
	int (*close)	__P((DB *, u_int32_t));
	int (*cursor)	__P((DB *, DB_TXN *, DBC **));
	int (*del)	__P((DB *, DB_TXN *, DBT *, u_int32_t));
	int (*fd)	__P((DB *, int *));
	int (*get)	__P((DB *, DB_TXN *, DBT *, DBT *, u_int32_t));
	int (*put)	__P((DB *, DB_TXN *, DBT *, DBT *, u_int32_t));
	int (*stat)	__P((DB *, void *, void *(*)(size_t), u_int32_t));
	int (*sync)	__P((DB *, u_int32_t));

#define	DB_AM_DUP	0x000001	/* DB_DUP (internal). */
#define	DB_AM_INMEM	0x000002	/* In-memory; no sync on close. */
#define	DB_AM_LOCKING	0x000004	/* Perform locking. */
#define	DB_AM_LOGGING	0x000008	/* Perform logging. */
#define	DB_AM_MLOCAL	0x000010	/* Database memory pool is local. */
#define	DB_AM_PGDEF	0x000020	/* Page size was defaulted. */
#define	DB_AM_RDONLY	0x000040	/* Database is readonly. */
#define	DB_AM_RECOVER	0x000080	/* In recovery (do not log or lock). */
#define	DB_AM_SWAP	0x000100	/* Pages need to be byte-swapped. */
#define	DB_AM_THREAD	0x000200	/* DB is multi-threaded. */
#define	DB_BT_RECNUM	0x000400	/* DB_RECNUM (internal) */
#define	DB_HS_DIRTYMETA 0x000800	/* Hash: Metadata page modified. */
#define	DB_RE_DELIMITER	0x001000	/* DB_DELIMITER (internal). */
#define	DB_RE_FIXEDLEN	0x002000	/* DB_FIXEDLEN (internal). */
#define	DB_RE_PAD	0x004000	/* DB_PAD (internal). */
#define	DB_RE_RENUMBER	0x008000	/* DB_RENUMBER (internal). */
#define	DB_RE_SNAPSHOT	0x010000	/* DB_SNAPSHOT (internal). */
	u_int32_t flags;
};

/* Cursor description structure. */
struct __dbc {
	DB *dbp;			/* Related DB access method. */
	DB_TXN	 *txn;			/* Associated transaction. */

	/*
	 * XXX
	 * Explicit representations of structures in queue.h.
	 *
	 * TAILQ_ENTRY(__dbc);
	 */
	struct {
		struct __dbc *tqe_next;
		struct __dbc **tqe_prev;
	} links;

	void	 *internal;		/* Access method private. */

	int (*c_close)	__P((DBC *));
	int (*c_del)	__P((DBC *, u_int32_t));
	int (*c_get)	__P((DBC *, DBT *, DBT *, u_int32_t));
	int (*c_put)	__P((DBC *, DBT *, DBT *, u_int32_t));
};

/* Btree/recno statistics structure. */
struct __db_bt_stat {
	u_int32_t bt_flags;		/* Open flags. */
	u_int32_t bt_maxkey;		/* Maxkey value. */
	u_int32_t bt_minkey;		/* Minkey value. */
	u_int32_t bt_re_len;		/* Fixed-length record length. */
	u_int32_t bt_re_pad;		/* Fixed-length record pad. */
	u_int32_t bt_pagesize;		/* Page size. */
	u_int32_t bt_levels;		/* Tree levels. */
	u_int32_t bt_nrecs;		/* Number of records. */
	u_int32_t bt_int_pg;		/* Internal pages. */
	u_int32_t bt_leaf_pg;		/* Leaf pages. */
	u_int32_t bt_dup_pg;		/* Duplicate pages. */
	u_int32_t bt_over_pg;		/* Overflow pages. */
	u_int32_t bt_free;		/* Pages on the free list. */
	u_int32_t bt_freed;		/* Pages freed for reuse. */
	u_int32_t bt_int_pgfree;	/* Bytes free in internal pages. */
	u_int32_t bt_leaf_pgfree;	/* Bytes free in leaf pages. */
	u_int32_t bt_dup_pgfree;	/* Bytes free in duplicate pages. */
	u_int32_t bt_over_pgfree;	/* Bytes free in overflow pages. */
	u_int32_t bt_pfxsaved;		/* Bytes saved by prefix compression. */
	u_int32_t bt_split;		/* Total number of splits. */
	u_int32_t bt_rootsplit;		/* Root page splits. */
	u_int32_t bt_fastsplit;		/* Fast splits. */
	u_int32_t bt_added;		/* Items added. */
	u_int32_t bt_deleted;		/* Items deleted. */
	u_int32_t bt_get;		/* Items retrieved. */
	u_int32_t bt_cache_hit;		/* Hits in fast-insert code. */
	u_int32_t bt_cache_miss;	/* Misses in fast-insert code. */
	u_int32_t bt_magic;		/* Magic number. */
	u_int32_t bt_version;		/* Version number. */
};

#if defined(__cplusplus)
extern "C" {
#endif
int   db_appinit __P((const char *, char * const *, DB_ENV *, u_int32_t));
int   db_appexit __P((DB_ENV *));
int   db_jump_set __P((void *, int));
int   db_open __P((const char *,
	  DBTYPE, u_int32_t, int, DB_ENV *, DB_INFO *, DB **));
int   db_value_set __P((int, int));
char *db_version __P((int *, int *, int *));
#if defined(__cplusplus)
}
#endif

/*******************************************************
 * Locking
 *******************************************************/
#define	DB_LOCKVERSION	1
#define	DB_LOCKMAGIC	0x090193

/* Flag values for lock_vec(). */
#define	DB_LOCK_NOWAIT		0x01	/* Don't wait on unavailable lock. */

/* Flag values for lock_detect(). */
#define	DB_LOCK_CONFLICT	0x01	/* Run on any conflict. */

/*
 * Request types.
 *
 * XXX
 * Changes here must be reflected in java/src/com/sleepycat/db/Db.java.
 */
typedef enum {
	DB_LOCK_DUMP=0,			/* Display held locks. */
	DB_LOCK_GET,			/* Get the lock. */
	DB_LOCK_PUT,			/* Release the lock. */
	DB_LOCK_PUT_ALL,		/* Release locker's locks. */
	DB_LOCK_PUT_OBJ			/* Release locker's locks on obj. */
} db_lockop_t;

/*
 * Simple R/W lock modes and for multi-granularity intention locking.
 *
 * XXX
 * Changes here must be reflected in java/src/com/sleepycat/db/Db.java.
 */
typedef enum {
	DB_LOCK_NG=0,			/* Not granted. */
	DB_LOCK_READ,			/* Shared/read. */
	DB_LOCK_WRITE,			/* Exclusive/write. */
	DB_LOCK_IREAD,			/* Intent to share/read. */
	DB_LOCK_IWRITE,			/* Intent exclusive/write. */
	DB_LOCK_IWR			/* Intent to read and write. */
} db_lockmode_t;

/*
 * Status of a lock.
 */
typedef enum {
	DB_LSTAT_ABORTED,		/* Lock belongs to an aborted txn. */
	DB_LSTAT_ERR,			/* Lock is bad. */
	DB_LSTAT_FREE,			/* Lock is unallocated. */
	DB_LSTAT_HELD,			/* Lock is currently held. */
	DB_LSTAT_NOGRANT,		/* Lock was not granted. */
	DB_LSTAT_PENDING,		/* Lock was waiting and has been
					 * promoted; waiting for the owner
					 * to run and upgrade it to held. */
	DB_LSTAT_WAITING		/* Lock is on the wait queue. */
} db_status_t;

/* Lock request structure. */
struct __db_lockreq {
	db_lockop_t	 op;		/* Operation. */
	db_lockmode_t	 mode;		/* Requested mode. */
	u_int32_t	 locker;	/* Locker identity. */
	DBT		*obj;		/* Object being locked. */
	DB_LOCK		 lock;		/* Lock returned. */
};

/*
 * Commonly used conflict matrices.
 *
 * Standard Read/Write (or exclusive/shared) locks.
 */
#define	DB_LOCK_RW_N	3
extern const u_int8_t db_rw_conflicts[];

/* Multi-granularity locking. */
#define	DB_LOCK_RIW_N	6
extern const u_int8_t db_riw_conflicts[];

struct __db_lock_stat {
	u_int32_t st_magic;		/* Lock file magic number. */
	u_int32_t st_version;		/* Lock file version number. */
	u_int32_t st_maxlocks;		/* Maximum number of locks in table. */
	u_int32_t st_nmodes;		/* Number of lock modes. */
	u_int32_t st_numobjs;		/* Number of objects. */
	u_int32_t st_nlockers;		/* Number of lockers. */
	u_int32_t st_nconflicts;	/* Number of lock conflicts. */
	u_int32_t st_nrequests;		/* Number of lock gets. */
	u_int32_t st_nreleases;		/* Number of lock puts. */
	u_int32_t st_ndeadlocks;	/* Number of lock deadlocks. */
	u_int32_t st_region_wait;	/* Region lock granted after wait. */
	u_int32_t st_region_nowait;	/* Region lock granted without wait. */
	u_int32_t st_refcnt;		/* Region reference count. */
	u_int32_t st_regsize;		/* Region size. */
};

#if defined(__cplusplus)
extern "C" {
#endif
int	  lock_close __P((DB_LOCKTAB *));
int	  lock_detect __P((DB_LOCKTAB *, u_int32_t, u_int32_t));
int	  lock_get __P((DB_LOCKTAB *,
	    u_int32_t, u_int32_t, const DBT *, db_lockmode_t, DB_LOCK *));
int	  lock_id __P((DB_LOCKTAB *, u_int32_t *));
int	  lock_open __P((const char *,
	    u_int32_t, int, DB_ENV *, DB_LOCKTAB **));
int	  lock_put __P((DB_LOCKTAB *, DB_LOCK));
int	  lock_stat __P((DB_LOCKTAB *, DB_LOCK_STAT **, void *(*)(size_t)));
int	  lock_unlink __P((const char *, int, DB_ENV *));
int	  lock_vec __P((DB_LOCKTAB *,
	    u_int32_t, u_int32_t, DB_LOCKREQ *, int, DB_LOCKREQ **));
#if defined(__cplusplus)
}
#endif

/*******************************************************
 * Logging.
 *******************************************************/
/* Flag values for log_archive(). */
#define	DB_ARCH_ABS		0x001	/* Absolute pathnames. */
#define	DB_ARCH_DATA		0x002	/* Data files. */
#define	DB_ARCH_LOG		0x004	/* Log files. */

/*
 * A DB_LSN has two parts, a fileid which identifies a specific file, and an
 * offset within that file.  The fileid is an unsigned 4-byte quantity that
 * uniquely identifies a file within the log directory -- currently a simple
 * counter inside the log.  The offset is also an unsigned 4-byte value.  The
 * log manager guarantees the offset is never more than 4 bytes by switching
 * to a new log file before the maximum length imposed by an unsigned 4-byte
 * offset is reached.
 */
struct __db_lsn {
	u_int32_t	file;		/* File ID. */
	u_int32_t	offset;		/* File offset. */
};

/* Log statistics structure. */
struct __db_log_stat {
	u_int32_t st_magic;		/* Log file magic number. */
	u_int32_t st_version;		/* Log file version number. */
	int st_mode;			/* Log file mode. */
	u_int32_t st_lg_max;		/* Maximum log file size. */
	u_int32_t st_w_bytes;		/* Bytes to log. */
	u_int32_t st_w_mbytes;		/* Megabytes to log. */
	u_int32_t st_wc_bytes;		/* Bytes to log since checkpoint. */
	u_int32_t st_wc_mbytes;		/* Megabytes to log since checkpoint. */
	u_int32_t st_wcount;		/* Total syncs to the log. */
	u_int32_t st_scount;		/* Total writes to the log. */
	u_int32_t st_region_wait;	/* Region lock granted after wait. */
	u_int32_t st_region_nowait;	/* Region lock granted without wait. */
	u_int32_t st_cur_file;		/* Current log file number. */
	u_int32_t st_cur_offset;	/* Current log file offset. */
	u_int32_t st_refcnt;		/* Region reference count. */
	u_int32_t st_regsize;		/* Region size. */
};

#if defined(__cplusplus)
extern "C" {
#endif
int	 log_archive __P((DB_LOG *, char **[], u_int32_t, void *(*)(size_t)));
int	 log_close __P((DB_LOG *));
int	 log_compare __P((const DB_LSN *, const DB_LSN *));
int	 log_file __P((DB_LOG *, const DB_LSN *, char *, size_t));
int	 log_flush __P((DB_LOG *, const DB_LSN *));
int	 log_get __P((DB_LOG *, DB_LSN *, DBT *, u_int32_t));
int	 log_open __P((const char *, u_int32_t, int, DB_ENV *, DB_LOG **));
int	 log_put __P((DB_LOG *, DB_LSN *, const DBT *, u_int32_t));
int	 log_register __P((DB_LOG *, DB *, const char *, DBTYPE, u_int32_t *));
int	 log_stat __P((DB_LOG *, DB_LOG_STAT **, void *(*)(size_t)));
int	 log_unlink __P((const char *, int, DB_ENV *));
int	 log_unregister __P((DB_LOG *, u_int32_t));
#if defined(__cplusplus)
}
#endif

/*******************************************************
 * Mpool
 *******************************************************/
/* Flag values for memp_fget(). */
#define	DB_MPOOL_CREATE		0x001	/* Create a page. */
#define	DB_MPOOL_LAST		0x002	/* Return the last page. */
#define	DB_MPOOL_NEW		0x004	/* Create a new page. */

/* Flag values for memp_fput(), memp_fset(). */
#define	DB_MPOOL_CLEAN		0x001	/* Clear modified bit. */
#define	DB_MPOOL_DIRTY		0x002	/* Page is modified. */
#define	DB_MPOOL_DISCARD	0x004	/* Don't cache the page. */

/* Mpool statistics structure. */
struct __db_mpool_stat {
	size_t st_cachesize;		/* Cache size. */
	u_int32_t st_cache_hit;		/* Pages found in the cache. */
	u_int32_t st_cache_miss;	/* Pages not found in the cache. */
	u_int32_t st_map;		/* Pages from mapped files. */
	u_int32_t st_page_create;	/* Pages created in the cache. */
	u_int32_t st_page_in;		/* Pages read in. */
	u_int32_t st_page_out;		/* Pages written out. */
	u_int32_t st_ro_evict;		/* Clean pages forced from the cache. */
	u_int32_t st_rw_evict;		/* Dirty pages forced from the cache. */
	u_int32_t st_hash_buckets;	/* Number of hash buckets. */
	u_int32_t st_hash_searches;	/* Total hash chain searches. */
	u_int32_t st_hash_longest;	/* Longest hash chain searched. */
	u_int32_t st_hash_examined;	/* Total hash entries searched. */
	u_int32_t st_page_clean;	/* Clean pages. */
	u_int32_t st_page_dirty;	/* Dirty pages. */
	u_int32_t st_page_trickle;	/* Pages written by memp_trickle. */
	u_int32_t st_region_wait;	/* Region lock granted after wait. */
	u_int32_t st_region_nowait;	/* Region lock granted without wait. */
	u_int32_t st_refcnt;		/* Region reference count. */
	u_int32_t st_regsize;		/* Region size. */
};

/* Mpool file open information structure. */
struct __db_mpool_finfo {
	int	   ftype;		/* File type. */
	DBT	  *pgcookie;		/* Byte-string passed to pgin/pgout. */
	u_int8_t  *fileid;		/* Unique file ID. */
	int32_t	   lsn_offset;		/* LSN offset in page. */
	u_int32_t  clear_len;		/* Cleared length on created pages. */
};

/* Mpool file statistics structure. */
struct __db_mpool_fstat {
	char *file_name;		/* File name. */
	size_t st_pagesize;		/* Page size. */
	u_int32_t st_cache_hit;		/* Pages found in the cache. */
	u_int32_t st_cache_miss;	/* Pages not found in the cache. */
	u_int32_t st_map;		/* Pages from mapped files. */
	u_int32_t st_page_create;	/* Pages created in the cache. */
	u_int32_t st_page_in;		/* Pages read in. */
	u_int32_t st_page_out;		/* Pages written out. */
};

#if defined(__cplusplus)
extern "C" {
#endif
int	memp_close __P((DB_MPOOL *));
int	memp_fclose __P((DB_MPOOLFILE *));
int	memp_fget __P((DB_MPOOLFILE *, db_pgno_t *, u_int32_t, void *));
int	memp_fopen __P((DB_MPOOL *, const char *,
	    u_int32_t, int, size_t, DB_MPOOL_FINFO *, DB_MPOOLFILE **));
int	memp_fput __P((DB_MPOOLFILE *, void *, u_int32_t));
int	memp_fset __P((DB_MPOOLFILE *, void *, u_int32_t));
int	memp_fsync __P((DB_MPOOLFILE *));
int	memp_open __P((const char *, u_int32_t, int, DB_ENV *, DB_MPOOL **));
int	memp_register __P((DB_MPOOL *, int,
	    int (*)(db_pgno_t, void *, DBT *),
	    int (*)(db_pgno_t, void *, DBT *)));
int	memp_stat __P((DB_MPOOL *,
	    DB_MPOOL_STAT **, DB_MPOOL_FSTAT ***, void *(*)(size_t)));
int	memp_sync __P((DB_MPOOL *, DB_LSN *));
int	memp_trickle __P((DB_MPOOL *, int, int *));
int	memp_unlink __P((const char *, int, DB_ENV *));
#if defined(__cplusplus)
}
#endif

/*******************************************************
 * Transactions.
 *******************************************************/
#define	DB_TXNVERSION	1
#define	DB_TXNMAGIC	0x041593

/* Operations values to the tx_recover() function. */
#define	DB_TXN_BACKWARD_ROLL	1	/* Read the log backwards. */
#define	DB_TXN_FORWARD_ROLL	2	/* Read the log forwards. */
#define	DB_TXN_OPENFILES	3	/* Read for open files. */
#define	DB_TXN_REDO		4	/* Redo the operation. */
#define	DB_TXN_UNDO		5	/* Undo the operation. */

/* Internal transaction status values. */

/* Transaction statistics structure. */
struct __db_txn_active {
	u_int32_t	txnid;		/* Transaction ID */
	DB_LSN		lsn;		/* Lsn of the begin record */
};

struct __db_txn_stat {
	DB_LSN	  st_last_ckp;		/* lsn of the last checkpoint */
	DB_LSN	  st_pending_ckp;	/* last checkpoint did not finish */
	time_t	  st_time_ckp;		/* time of last checkpoint */
	u_int32_t st_last_txnid;	/* last transaction id given out */
	u_int32_t st_maxtxns;	/* maximum number of active txns */
	u_int32_t st_naborts;	/* number of aborted transactions */
	u_int32_t st_nbegins;	/* number of begun transactions */
	u_int32_t st_ncommits;	/* number of committed transactions */
	u_int32_t st_nactive;	/* number of active transactions */
	DB_TXN_ACTIVE
		 *st_txnarray;	/* array of active transactions */
	u_int32_t st_region_wait;	/* Region lock granted after wait. */
	u_int32_t st_region_nowait;	/* Region lock granted without wait. */
	u_int32_t st_refcnt;		/* Region reference count. */
	u_int32_t st_regsize;		/* Region size. */
};

#if defined(__cplusplus)
extern "C" {
#endif
int	  txn_abort __P((DB_TXN *));
int	  txn_begin __P((DB_TXNMGR *, DB_TXN *, DB_TXN **));
int	  txn_checkpoint __P((const DB_TXNMGR *, u_int32_t, u_int32_t));
int	  txn_commit __P((DB_TXN *));
int	  txn_close __P((DB_TXNMGR *));
u_int32_t txn_id __P((DB_TXN *));
int	  txn_open __P((const char *, u_int32_t, int, DB_ENV *, DB_TXNMGR **));
int	  txn_prepare __P((DB_TXN *));
int	  txn_stat __P((DB_TXNMGR *, DB_TXN_STAT **, void *(*)(size_t)));
int	  txn_unlink __P((const char *, int, DB_ENV *));
#if defined(__cplusplus)
}
#endif

#ifndef DB_DBM_HSEARCH
#define	DB_DBM_HSEARCH	0		/* No historic interfaces by default. */
#endif
#if DB_DBM_HSEARCH != 0
/*******************************************************
 * Dbm/Ndbm historic interfaces.
 *******************************************************/
#define	DBM_INSERT	0		/* Flags to dbm_store(). */
#define	DBM_REPLACE	1

/*
 * The db(3) support for ndbm(3) always appends this suffix to the
 * file name to avoid overwriting the user's original database.
 */
#define	DBM_SUFFIX	".db"

#if defined(_XPG4_2)
typedef struct {
	char *dptr;
	size_t dsize;
} datum;
#else
typedef struct {
	char *dptr;
	int dsize;
} datum;
#endif

/*
 * Translate DBM calls into DB calls so that DB doesn't step on the
 * application's name space.
 *
 * The global variables dbrdonly, dirf and pagf were not retained when
 * 4BSD replaced the dbm interface with ndbm, and are not support here.
 */
#define	dbminit(a)	__db_dbm_init(a)
#if !defined(__cplusplus)
#define	delete(a)	__db_dbm_delete(a)
#endif
#define	fetch(a)	__db_dbm_fetch(a)
#define	firstkey	__db_dbm_firstkey
#define	nextkey(a)	__db_dbm_nextkey(a)
#define	store(a, b)	__db_dbm_store(a, b)

/* Prototype the DB calls. */
#if defined(__cplusplus)
extern "C" {
#endif
int	 __db_dbm_init __P((char *));
int	 __db_dbm_delete __P((datum));
int	 __db_dbm_dbrdonly __P((void));
int	 __db_dbm_dirf __P((void));
datum	 __db_dbm_fetch __P((datum));
datum	 __db_dbm_firstkey __P((void));
datum	 __db_dbm_nextkey __P((datum));
int	 __db_dbm_pagf __P((void));
int	 __db_dbm_store __P((datum, datum));
#if defined(__cplusplus)
}
#endif

/*
 * Translate NDBM calls into DB calls so that DB doesn't step on the
 * application's name space.
 */
#define	dbm_clearerr(a)		__db_ndbm_clearerr(a)
#define	dbm_close(a)		__db_ndbm_close(a)
#define	dbm_delete(a, b)	__db_ndbm_delete(a, b)
#define	dbm_dirfno(a)		__db_ndbm_dirfno(a)
#define	dbm_error(a)		__db_ndbm_error(a)
#define	dbm_fetch(a, b)		__db_ndbm_fetch(a, b)
#define	dbm_firstkey(a)		__db_ndbm_firstkey(a)
#define	dbm_nextkey(a)		__db_ndbm_nextkey(a)
#define	dbm_open(a, b, c)	__db_ndbm_open(a, b, c)
#define	dbm_pagfno(a)		__db_ndbm_pagfno(a)
#define	dbm_rdonly(a)		__db_ndbm_rdonly(a)
#define	dbm_store(a, b, c, d)	__db_ndbm_store(a, b, c, d)

/* Prototype the DB calls. */
#if defined(__cplusplus)
extern "C" {
#endif
int	 __db_ndbm_clearerr __P((DBM *));
void	 __db_ndbm_close __P((DBM *));
int	 __db_ndbm_delete __P((DBM *, datum));
int	 __db_ndbm_dirfno __P((DBM *));
int	 __db_ndbm_error __P((DBM *));
datum	 __db_ndbm_fetch __P((DBM *, datum));
datum	 __db_ndbm_firstkey __P((DBM *));
datum	 __db_ndbm_nextkey __P((DBM *));
DBM	*__db_ndbm_open __P((const char *, int, int));
int	 __db_ndbm_pagfno __P((DBM *));
int	 __db_ndbm_rdonly __P((DBM *));
int	 __db_ndbm_store __P((DBM *, datum, datum, int));
#if defined(__cplusplus)
}
#endif

/*******************************************************
 * Hsearch historic interface.
 *******************************************************/
typedef enum {
	FIND, ENTER
} ACTION;

typedef struct entry {
	char *key;
	char *data;
} ENTRY;

/*
 * Translate HSEARCH calls into DB calls so that DB doesn't step on the
 * application's name space.
 */
#define	hcreate(a)	__db_hcreate(a)
#define	hdestroy	__db_hdestroy
#define	hsearch(a, b)	__db_hsearch(a, b)

/* Prototype the DB calls. */
#if defined(__cplusplus)
extern "C" {
#endif
int	 __db_hcreate __P((size_t));
void	 __db_hdestroy __P((void));
ENTRY	*__db_hsearch __P((ENTRY, ACTION));
#if defined(__cplusplus)
}
#endif
#endif /* DB_DBM_HSEARCH */

/*
 * XXX
 * MacOS: Reset Metrowerks C enum sizes.
 */
#ifdef __MWERKS__
#pragma enumsalwaysint reset
#endif
#endif /* !_DB_H_ */
