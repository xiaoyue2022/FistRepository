
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h> 
#include <wait.h> 
#include <sys/types.h>
#include<stdlib.h> 
void sigchld_handler(int sig)
{
    int status;
    waitpid(-1,&status, 0) ;
 if  ( WIFEXITED(status) )  printf("child process exit normally\n");
 else if ( WIFSIGNALED(status) ) printf("child process exit abnormally\n");
 else if ( WIFSTOPPED(status) ) printf("child process is stopped\n");
  else printf("else");
}
int main()
{
   pid_t pid;
      signal(SIGCHLD,sigchld_handler) ;
   pid=fork() ;
   if (pid==0) abort() ;  
   else if (pid>0) {sleep(2); printf("parent process\n");}
   else exit(0);
}
