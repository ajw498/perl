/* stat.h   Author S.W.Ellacott@brighton.ac.uk */

typedef unsigned int	dev_t;
typedef int		ino_t;
typedef unsigned int	time_t;
/*    typedef unsigned int	off_t;     */

struct stat {
  dev_t 	        st_dev;
  ino_t 		st_ino;
  unsigned int 		st_mode;
  int			st_nlink;
  unsigned short	st_uid;
  unsigned short	st_gid;
  dev_t 		st_rdev;
  unsigned int 		st_size;
  unsigned int 		st_blksize;
  time_t		st_atime;
  time_t		st_mtime;
  time_t		st_ctime;
  };
