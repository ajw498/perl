/** gdbm as a riscos module **/
/** S.E.Ellacott@brighton.ac.uk **/
/** Fri 11th July 1997 **/

#include <stdio.h>
#include <kernel.h>
#include <swis.h>
#include <string.h>
#include "autoconf.h"
#include "gdbmdefs.h"
#include "gdbmerrno.h"
#include "extern.h"
#include "getopt.h"
#include "fcntl.h"

static gdbm_file_info * gdbm_file;
static datum key, value, lastkey;
static unsigned int tempint;
struct op_file {
  gdbm_file_info * handle;
  struct op_file * next_file;
};
static struct op_file *top_file, *this_file, *del_file, *temp_file; /* Keep a list of open files */
static  _kernel_oserror mem_error;
 
void gdbm_final() {
  /* Close all open files */
  this_file = top_file;
  while ( this_file->next_file != NULL ) {
    gdbm_close( this_file->handle );
    this_file = this_file->next_file;
  }
}

_kernel_oserror * gdbm_init(char *cmd_tail, int podule_base, void *reg_12)
{
  top_file = (struct op_file *) malloc( sizeof(struct op_file) );
  this_file = top_file;
  if ( top_file == NULL ) {
    mem_error.errnum = 255;
    strcpy(mem_error.errmess,"No memory to initialise gdbm");
    return &mem_error;
  }
  this_file->handle = NULL;
  this_file->next_file = NULL;
  atexit(gdbm_final);
 return 0;
}


_kernel_oserror * gdbm_swi(int swi_no, _kernel_swi_regs * regs, void *reg_12)
{
   switch (swi_no) {
    case 0:
    	 /* On entry, file name is in reg 0.
    	    On exit, returns the db handle in reg 0 */
    	 gdbm_file = gdbm_open ((char *) regs->r[0], 0, GDBM_WRCREAT, 00664, NULL);
    	 if ( gdbm_file != 0 ) {
    	   this_file->handle = gdbm_file;
    	   this_file->next_file = (struct op_file *) malloc( sizeof(struct op_file) );
    	   if ( this_file->next_file == NULL ) {
             mem_error.errnum = 255;
             strcpy(mem_error.errmess,"Out of memory in gdbm");
    	     return &mem_error;
    	   }
    	   this_file = this_file->next_file;
    	   this_file->handle = NULL;
    	   this_file->next_file = NULL;
    	 }
	 regs->r[0]  = (int) gdbm_file;
    	 break;

    case 1:
    	 /* On entry handle is in reg 0, key string in reg 1, length in reg 2
    	             value string in reg 3 length in reg 4 */
    	 if ( !regs->r[0] ) break;
	 key.dptr = (char *) regs->r[1];
	 key.dsize = regs->r[2];
	 value.dptr = (char *) regs->r[3];
	 value.dsize = regs->r[4];
	 gdbm_store( (gdbm_file_info *) regs->r[0] , key, value, GDBM_REPLACE);
    	 break;

    case 2:
    	 /* On entry handle is in reg 0, key string in reg 1, length in reg 2,
    	    address of value string in reg 3 length in reg 4.
    	    On exit, actual length of item is in reg 0: if greater than value
    	    string length, the item has been truncated. If the item does not exist
    	    reg 0 is set to -1.                                                      */
	 if ( !regs->r[0] ) break;
	 key.dptr = (char *) regs->r[1];
	 key.dsize = regs->r[2];
    	 value = gdbm_fetch( (gdbm_file_info *) regs->r[0], key);
    	 if ( !value.dptr ) { regs->r[0] = -1; break; };
    	 tempint = regs->r[4];
    	 if ( tempint >value.dsize ) tempint = value.dsize;
    	 memcpy( (char *) regs->r[3], value.dptr, tempint);
    	 regs->r[0] = value.dsize;
    	 free( value.dptr );
	 break;

    case 3:
    	 /* On entry handle is in reg 0, key string in reg 1, length in reg 2,
            If the item does not exist reg 0 is set to 0.                   */
	 if ( !regs->r[0] ) break;
	 key.dptr = (char *) regs->r[1];
	 key.dsize = regs->r[2];
    	 regs->r[0] = gdbm_exists( (gdbm_file_info *) regs->r[0], key);
	 break;

    case 4:
    	 /* On entry handle is in reg 0, key buffer in reg 1, length in reg 2.
            Actual length copied is returned in reg[0], or -1 if no first key */
    	 if ( !regs->r[0] ) break;
    	 key = gdbm_firstkey( (gdbm_file_info *) regs->r[0] );
    	 if ( !key.dptr ) { regs->r[0] = -1; break; }
    	 tempint = regs->r[2];
    	 if ( tempint > key.dsize ) tempint = key.dsize;
    	 memcpy( (char *) regs->r[1], key.dptr, tempint);
    	 regs->r[0] = key.dsize;
    	 free(key.dptr);
	 break;

    case 5:
            /* On entry handle is in reg 0, key string in reg 1, length in reg 2,
    	    address of reply string in reg 3 length in reg 4.
    	    On exit, actual length of item is in reg 0: if greater than value
    	    string length, the item has been truncated. If the list is finished, -1
    	    is returned in reg 0*/
         if ( !regs->r[0] ) break;
         lastkey.dptr = (char *) regs->r[1];
	 lastkey.dsize = regs->r[2];
    	 key = gdbm_nextkey( (gdbm_file_info *)  regs->r[0], lastkey  );
    	 if ( !key.dptr ) { regs->r[0] = -1; break; }
    	 tempint = regs->r[4];
    	 if ( tempint > key.dsize ) tempint = key.dsize;
    	 memcpy( (char *) regs->r[3], key.dptr, tempint);
    	 regs->r[0] = key.dsize;
    	 free(key.dptr);
	 break;

    case 6:
    	 /* Closes the db whose handle is in reg 0 */
    	 if ( !regs->r[0] ) break;
    	 del_file = top_file;
    	 temp_file = del_file;
    	 if ( del_file->handle == (gdbm_file_info *)  regs->r[0] ) {
    	   top_file = del_file->next_file;
    	   free( del_file );
    	 }
    	 else while ( del_file->next_file != NULL ) {
    	   del_file = del_file->next_file;
    	   if ( del_file->handle == (gdbm_file_info *)  regs->r[0] ) {
    	     temp_file->next_file = del_file->next_file;
    	     free( del_file );
    	     break;
       	   }
    	   temp_file = del_file;
    	 }
    	 gdbm_close( (gdbm_file_info *)  regs->r[0] );
	 break;

    case 7:
    	 /* On entry handle is in reg 0, key string in reg 1, length in reg 2,
            If the item does not exist reg 0 is set to -1, or 0 for a successful
            delete.                                                             */
         if ( !regs->r[0] ) break;
         key.dptr = (char *) regs->r[1];
	 key.dsize = regs->r[2];
    	 regs->r[0] = gdbm_delete( (gdbm_file_info *) regs->r[0], key);
	 break;

    case 8:
         /* Clears the database whose handle is in reg 0 */
    	 if ( !regs->r[0] ) break;
    	 value = gdbm_firstkey( (gdbm_file_info *) regs->r[0]);
    	 if ( !value.dptr ) break;
    	 while ( value.dptr ) {
    	   key =  gdbm_nextkey( (gdbm_file_info *) regs->r[0], value );
    	   gdbm_delete( (gdbm_file_info *) regs->r[0], value);
    	   free( value.dptr );
    	   value = key;
    	 }
	 break;
	 
	 case 9:
    	 /* On entry, file name is in reg 0. Opens the db for reading only.
    	    On exit, returns the db handle in reg 0 */
    	 gdbm_file = gdbm_open ((char *) regs->r[0], 0, GDBM_READER, 00664, NULL);
    	 if ( gdbm_file != 0 ) {
    	   this_file->handle = gdbm_file;
    	   this_file->next_file = (struct op_file *) malloc( sizeof(struct op_file) );
    	   if ( this_file->next_file == NULL ) {
             mem_error.errnum = 255;
             strcpy(mem_error.errmess,"Out of memory in gdbm");
    	     return &mem_error;
    	   }
    	   this_file = this_file->next_file;
    	   this_file->handle = NULL;
    	   this_file->next_file = NULL;
    	 }
	 regs->r[0]  = (int) gdbm_file;
    	 break;

  }
  return 0;
}

