/*mypipe.c*/
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include <stdlib.h>
void main() {
int pid ,mypipe[2];
pipe(mypipe);
pid=fork() ;
if (pid<0 ) {
    perror("create process failed\n");
    exit(0);
}
 if (pid==0) {  //子进程
   close(mypipe[1]);
   dup2(mypipe[0],0);
   close(mypipe[0]);
   sleep(1);
   execlp("grep","grep","init",NULL);
 }
 else {  //父进程
  close(mypipe[0]);
  dup2(mypipe[1],1);
  close(mypipe[1]);
  execlp("ps","ps","aux",NULL);
}
}
