#include <sys/types.h>   
#include <unistd.h>      
#include <fcntl.h>
main()
{
  int fd;
  struct flock lock, savelock;

  fd = open("book.dat", O_RDWR);
  lock.l_type    = F_WRLCK;   /* Test for any lock on any part of file. */
  lock.l_start   = 0;
  lock.l_whence  = SEEK_SET;
  lock.l_len     = 20;        
  savelock = lock;
  fcntl(fd, F_GETLK, &lock);  /*获得当前的文件锁信息*/
  if (lock.l_type == F_WRLCK)
  {
     printf("Process %ld has a write lock already!\n", lock.l_pid);
     exit(1);
  }
  else if (lock.l_type == F_RDLCK)
  {
     printf("Process %ld has a read lock already!\n", lock.l_pid);
     exit(1);
  }
  else
     fcntl(fd, F_SETLK, &savelock);   //
  pause();
}
