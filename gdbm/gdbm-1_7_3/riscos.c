/* riscos.c */
/*    A riscos veneer (mostly file handling) for gdbm
      by S.W.Ellacott@brighton.ac.uk  Wed 18th June 1997
      This version uses low level calls to avoid the 16 file
      limit of Clib.                                        */

#include <stdio.h>
#include "stat.h"
#include <kernel.h>

#define __IORO 0x4E
#define __IOCR 0x86
#define __IORW 0xC6

static _kernel_osgbpb_block _gbpb_block;

int open(char* name, int type, int chmod)
/* chmod is not used at present
   type is zero if "read only"   */

   {
   int res;
   if (!type) {
     res = _kernel_osfind(__IORO, name);
     if (!res) res = -1;
   }
   else { 
     res = _kernel_osfind(__IORW, name);
     if (!res) res = _kernel_osfind(__IOCR, name); /* Try and create it */
     if (!res) res = -1;
   }
   return res;
   }

int close(int handle)
   {
   return _kernel_osfind(0, (void *) handle);
   }

int write(int handle, void * ptr, int count)
   {
    _gbpb_block.dataptr = ptr;
    _gbpb_block.nbytes = count;
    if ( _kernel_osgbpb(2, (unsigned) handle, &_gbpb_block) ) return count;
    else return 0;
   }

int read(int handle, void * ptr, int count)
   {
    _gbpb_block.dataptr = ptr;
    _gbpb_block.nbytes = count;
   _kernel_osgbpb(4, handle, &_gbpb_block);
   return count - _gbpb_block.nbytes;
   }

int fsync(int handle)
   {
   return _kernel_osargs(255, handle, 0);
   }

int lseek(int handle, long int offset, int wherefrom)
   {
    _kernel_osargs(1, handle, (int) offset);
    return offset;
   }

void fstat(int handle, struct stat * file_stat)
/* Not much of this is used by gdbm: only the file size st_size
   in gdbmopen and the mode st_mode in gdbmreorg (I hope! SWE).
   Note: no check is done for existence of the file.           */

{

   /* The file mode is not used at present */
   file_stat->st_mode = 0;


   file_stat->st_size = _kernel_osargs(2, handle, 0);

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
