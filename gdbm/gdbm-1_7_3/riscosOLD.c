/* riscos.c */
/*    A riscos veneer (mostly file handling) for gdbm
      by S.W.Ellacott@brighton.ac.uk
      Most is standard ANSI C.                       */

#include <stdio.h>
#include "stat.h"

int open(char* name, int type, int chmod)
/* chmod is not used at present
   type is zero if "read only"   */

   {
   int res;
   if ( !type ) res = (int) fopen(name, "rb");
   else res = (int) fopen(name, "ab+");
   if (!res) res = -1;
   else fseek( (FILE *) res, 0, 0);
   return res;
   }

int close(int handle)
   {
   return fclose( (FILE *) handle);
   }

int write(int handle, void * ptr, int count)
   {
   return fwrite( ptr, sizeof(char), count, (FILE *) handle);
   }

int read(int handle, void * ptr, int count)
   {
   return fread( ptr, sizeof(char), count, (FILE *) handle);
   }

int fsync(int handle)
   {
   return (int) fflush( (FILE *) handle);
   }

int lseek(int handle, long int offset, int wherefrom)
   {
   if ( fseek( (FILE *) handle, offset, wherefrom ) ) return -2;
   else return ftell( (FILE *) handle );
   }

void fstat(int handle, struct stat * file_stat)
/* Not much of this is used by gdbm: only the file size st_size
   in gdbmopen and the mode st_mode in gdbmreorg (I hope! SWE).
   Note: no check is done for existence of the file.           */

   {
   long int c_pos;

   /* The file mode is not used at present */
   file_stat->st_mode = 0;

   /* Find the length using ANSI C - Not easy!!!
      This might only work for binary files.    */
   fflush( (FILE *) handle);
   c_pos = ftell( (FILE *) handle);
   fseek( (FILE *) handle, 0L, 2); /* 2 is SEEK_END */
   file_stat->st_size = ftell( (FILE *) handle);
   fseek( (FILE *) handle, c_pos, 0); /* Return to previous position */
   }

   /* No file locking implemented at present.
      flock just returns the "success" condition */
   int flock(int handle, int action)
   {
     switch ( action ) {
       case 8: ;/* UNLOCK */
       	       break;
       case 5: ; /*  READLOCK */
       	       break;
       case 6: ; /* WRITELOCK */
       	       break;
       default:printf("flock called: UNKNOWN\n"); return 1;
     }

     return 0;
   }
