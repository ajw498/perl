/* gdbmreorg.c - Reorganize the database file. */

/**** A DUMMY BY S. ELLACOTT FOR RISCOS ****/


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

/* include system configuration before all else. */
#include "autoconf.h"

#include "gdbmdefs.h"
#include "gdbmerrno.h"
#include "extern.h"

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

      gdbm_errno = GDBM_REORGANIZE_FAILED;
      return -1;
}
