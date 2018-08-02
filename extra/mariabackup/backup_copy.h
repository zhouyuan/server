
#ifndef XTRABACKUP_BACKUP_COPY_H
#define XTRABACKUP_BACKUP_COPY_H

#include <my_global.h>
#include "datasink.h"

/* special files */
#define XTRABACKUP_SLAVE_INFO "xtrabackup_slave_info"
#define XTRABACKUP_GALERA_INFO "xtrabackup_galera_info"
#define XTRABACKUP_BINLOG_INFO "xtrabackup_binlog_info"
#define XTRABACKUP_INFO "xtrabackup_info"

extern bool binlog_locked;

bool
backup_file_printf(const char *filename, const char *fmt, ...)
		ATTRIBUTE_FORMAT(printf, 2, 0);

/************************************************************************
Return true if first and second arguments are the same path. */
bool
equal_paths(const char *first, const char *second);

/************************************************************************
Copy file for backup/restore.
@return true in case of success. */
bool
copy_file(ds_ctxt_t *datasink,
	  const char *src_file_path,
	  const char *dst_file_path,
	  uint thread_n);

/** Report an operation to create, delete, or rename a file during backup.
@param[in]	space_id	tablespace identifier
@param[in]	flags		tablespace flags (NULL if not create)
@param[in]	name		file name (not NUL-terminated)
@param[in]	len		length of name, in bytes
@param[in]	new_name	new file name (NULL if not rename)
@param[in]	new_len		length of new_name, in bytes (0 if NULL) */
void backup_file_op(ulint space_id, const byte* flags,
		    const byte* name, ulint len,
		    const byte* new_name, ulint new_len);
/** Start --backup */
bool backup_start();
/** Release resources after backup_start() */
void backup_release();
/** Finish after backup_start() and backup_release() */
bool backup_finish();
bool
apply_log_finish();
bool
copy_back();
bool
decrypt_decompress();
bool
is_path_separator(char);
bool
directory_exists(const char *dir, bool create);

#endif
