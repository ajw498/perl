/* autoconf.h.in.  For RiscOs port set up manually by S. Ellacott.  */

/* Compling with Acorn compiler in -pcc mode at the moment */
#define IN_GCC 0
#define __STDC__ 0

/* Define if using alloca.c.  */
#define C_ALLOCA 1
/* For the moment. Use of flex might be better*/

/* Define to empty if the keyword does not work.  */
#undef const

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
#undef CRAY_STACKSEG_END

/* Define if you have alloca.h and it should be used (not Ultrix).  */
#undef HAVE_ALLOCA_H

/* Define if your struct stat has st_blksize.  */
#define HAVE_ST_BLKSIZE 0

/* Define if int is 16 bits instead of 32.  */
#define INT_16_BITS 0

/* Define if long int is 64 bits.  */
#define LONG_64_BITS 0

/* Define to `long' if <sys/types.h> doesn't define.  */
#define off_t long

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown
 */
#undef STACK_DIRECTION

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
#undef WORDS_BIGENDIAN

/* Define if you have bcopy.  */
#define HAVE_BCOPY 0

/* Define if you have flock.  */
#define HAVE_FLOCK 0

/* Define if you have fsync.  */
#define HAVE_FSYNC 1

/* Define if you have ftruncate.  */
#define HAVE_FTRUNCATE 0

/* Define if you have rename.  */
#define HAVE_RENAME 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 0

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 0

/* Define if you have the <stdlib.h> header file.  */
#define HAVE_STDLIB_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 0

/* Define if you have the <sys/stat.h> header file.  */
#define HAVE_SYS_STAT_H 0

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 0

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 0

/* Define if you have the dbm library (-ldbm).  */
#undef HAVE_LIBDBM

/* Define if you have the ndbm library (-lndbm).  */
#undef HAVE_LIBNDBM
