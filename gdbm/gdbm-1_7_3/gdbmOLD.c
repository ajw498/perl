/** gdbm as a module **/

#include <stdio.h>
#include <kernel.h>
#include <swis.h>
#include "autoconf.h"
#include "gdbmdefs.h"
#include "gdbmerrno.h"
#include "extern.h"
#include "getopt.h"
#include "fcntl.h"

static gdbm_file_info * gdbm_file;
static gdbm_file_info * keyfile;
static gdbm_file_info * valfile;
static datum key, value;
static char buffer[20];
static char * temp;
static unsigned int tempint, handle;

static int parsargs(char * * arg_vec)
{
    	 tempint = (unsigned int) strtoul(* arg_vec, &temp, 0);
    	 * arg_vec = temp + 1;
    	 return tempint;
}

static void sendback(char * str)
{
  printf("%s", str);
}

static int setkey(unsigned int handle, char * val)
{
    	 key.dptr = val;
    	 return key.dsize = strlen(key.dptr) + 1; /* remember the null */
}

static int setval(unsigned int handle, char * val)
{
    	 value.dptr = val;
    	 return value.dsize = strlen(value.dptr) + 1; /* remember the null */
}

_kernel_oserror * gdbm_init(char *cmd_tail, int podule_base, void *reg_12)
{
  printf("Initialising gdbm\n\n");
  keyfile =gdbm_open("<Wimp$ScrapDir>.gdbmkeys",0,GDBM_WRCREAT,00664,NULL);
  valfile =gdbm_open("<Wimp$ScrapDir>.gdbmvals",0,GDBM_WRCREAT,00664,NULL);
  return 0;
}

_kernel_oserror * gdbm_cmd(char *arg_string, int argc, int cmd_no, void *reg_12)
{
   switch (cmd_no) {
    case 0:
	 gdbm_file = gdbm_open (arg_string, 0, GDBM_WRCREAT, 00664, NULL);
	 sprintf(buffer, "%d", (int) gdbm_file);
	 sendback(buffer);
	 break;

    case 1:
    	 handle = parsargs(&arg_string);
    	 gdbm_store( (gdbm_file_info *) handle, key, value, GDBM_REPLACE);
	 break;

    case 2:
    	 handle = parsargs(&arg_string);
    	 value = gdbm_fetch( (gdbm_file_info *) handle, key);
    	 if ( value.dptr ) sendback(value.dptr);
    	 free( value.dptr );
	 break;

    case 3:
    	 handle = parsargs(&arg_string);
    	 tempint = gdbm_exists( (gdbm_file_info *) handle, key);
    	 sprintf(buffer, "%d", tempint);
    	 sendback(buffer);
	 break;

    case 4:
    	 handle = parsargs(&arg_string);
	 setkey(handle, arg_string);
	 break;

    case 5:
    	 handle = parsargs(&arg_string);
	 setval(handle, arg_string);
	 break;

  }
  return 0;
}

int  main(int argc, char *argv[])
{
    return 0;
}
