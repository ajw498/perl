/* gdbmreorg.c - Reorganize the database file. */

/* This version rewritten by S.W.Ellacott@brighton.ac.uk to work
   with an ANSI C library. The original is in the source directory
   as GNUreorg.c */

/*  This file is part of GDBM, the GNU data base manager, by Philip A. Nelson.
    Copyright (C) 1990, 1991, 1993  Free Software Foundation, Inc.

    GDBM is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2, or (at your option)
    any later version.

    GDBM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GDBM; see the file COPYING.  If not, write to
    the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

    You may contact the author by:
       e-mail:  phil@cs.wwu.edu
      us-mail:  Philip A. Nelson
                Computer Science Department
                Western Washington University
                Bellingham, WA 98226

************************************************************************/


/* AIX demands this be the very first thing in the file. */
#if !defined(__GNUC__) && defined(_AIX)
 #pragma alloca
#endif

/* include system configuration before all else. */
#include "autoconf.h"

#include "gdbmdefs.h"
#include "gdbmerrno.h"
#include "extern.h"

#include <stdio.h>

/* Reorganize the database.  This requires creating a new file and inserting
   all the elements in the old file DBF into the new file.  The new file
   is then renamed to the same name as the old file and DBF is updated to
   contain all the correct information about the new file.  If an error
   is detected, the return value is negative.  The value zero is returned
   after a successful reorganization. */

int
gdbm_reorganize (dbf)
     gdbm_file_info *dbf;

{
  gdbm_file_info *new_dbf;		/* The new file. */
  char *new_name;			/* A temporary name. */
  char *tmp_name;			/* A temporary name. */
  datum key, nextkey, data;		/* For the sequential sweep. */
  struct stat fileinfo;			/* Information about the file. */
  int  index;				/* Use in moving the bucket cache. */
  int temp;

  /* Readers can not reorganize! */
  if (dbf->read_write == GDBM_READER)
    {
      gdbm_errno = GDBM_READER_CANT_REORGANIZE;
      return -1;
    }

  /* Initialize the gdbm_errno variable. */
  gdbm_errno = GDBM_NO_ERROR;

  /* Construct new name for temporary file. */
  new_name= tmpnam(NULL);

  /* Get the mode for the old file and open the new database.
     The "fast" mode is used because the reorganization will fail
     unless we create a complete copy of the database. */
  fstat (dbf->desc, &fileinfo);
  new_dbf = gdbm_open (new_name, dbf->header->block_size,
		       GDBM_WRCREAT | GDBM_FAST,
		       fileinfo.st_mode, dbf->fatal_err);

  if (new_dbf == NULL)
    {
      gdbm_errno = GDBM_REORGANIZE_FAILED;
      return -1;
    }


  /* For each item in the old database, add an entry in the new. */
  key = gdbm_firstkey (dbf);

  while (key.dptr != NULL)
    {
      data = gdbm_fetch (dbf, key);
      if (data.dptr != NULL)
 	{
	  /* Add the data to the new file. */
	  if (gdbm_store (new_dbf, key, data, GDBM_INSERT) != 0)
	    {
	      gdbm_close (new_dbf);
	      gdbm_errno = GDBM_REORGANIZE_FAILED;
	      remove (new_name);
	      return -1;
	    }
 	}
      else
 	{
	  /* ERROR! Abort and don't finish reorganize. */
	  gdbm_close (new_dbf);
	  gdbm_errno = GDBM_REORGANIZE_FAILED;
	  remove (new_name);
	  return -1;
 	}
      nextkey = gdbm_nextkey (dbf, key);
      free (key.dptr);
      free (data.dptr);
      key = nextkey;
    }

  /* Write everything. */
  _gdbm_end_update (new_dbf);
  gdbm_sync (new_dbf);


  /* Move the new file to old name. */

  tmp_name=tmpnam(NULL);
  temp = new_dbf->desc;
  fclose( (FILE *) dbf->desc);
  fclose( (FILE *)  new_dbf->desc);
  if (rename (dbf->name, tmp_name) != 0)
    {
      gdbm_errno = GDBM_REORGANIZE_FAILED;
      gdbm_close (new_dbf);
      remove(new_dbf->name);
      return -1;
    }

  if (rename (new_dbf->name, dbf->name) != 0)
    {
      gdbm_errno = GDBM_REORGANIZE_FAILED;
      gdbm_close (new_dbf);
      rename (tmp_name, dbf->name);
      return -1;
    }
    else remove (tmp_name);

  freopen(dbf->name, "ab+", (FILE *) temp);

  /* Fix up DBF to have the correct information for the new file. */
  UNLOCK_FILE(dbf);
  close (dbf->desc);
  free (dbf->header);
  free (dbf->dir);

  if (dbf->bucket_cache != NULL) {
    for (index = 0; index < dbf->cache_size; index++) {
      if (dbf->bucket_cache[index].ca_bucket != NULL)
	free (dbf->bucket_cache[index].ca_bucket);
      if (dbf->bucket_cache[index].ca_data.dptr != NULL)
	free (dbf->bucket_cache[index].ca_data.dptr);
    }
    free (dbf->bucket_cache);
  }

  dbf->desc           = new_dbf->desc;
  dbf->header         = new_dbf->header;
  dbf->dir            = new_dbf->dir;
  dbf->bucket         = new_dbf->bucket;
  dbf->bucket_dir     = new_dbf->bucket_dir;
  dbf->last_read      = new_dbf->last_read;
  dbf->bucket_cache   = new_dbf->bucket_cache;
  dbf->cache_size     = new_dbf->cache_size;
  dbf->header_changed    = new_dbf->header_changed;
  dbf->directory_changed = new_dbf->directory_changed;
  dbf->bucket_changed    = new_dbf->bucket_changed;
  dbf->second_changed    = new_dbf->second_changed;

  free (new_dbf);

  /* Make sure the new database is all on disk. */
  fsync (dbf->desc);

  /* Force the right stuff for a correct bucket cache. */
  dbf->cache_entry    = &dbf->bucket_cache[0];
  _gdbm_get_bucket (dbf, 0);

  return 0;
}
