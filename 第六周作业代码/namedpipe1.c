#include<stdio.h>  
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <fcntl.h>
#define N 20
int main(){
int fd= open("/home/lyq/mypipe",O_RDONLY|O_CREAT);
char buf[N];
   if(fd!=-1) {  
        printf("FIFO file is opened\n");  
    } 
    else {
       perror("open failed"); 
       exit(0);
   }
   if ( read(fd,buf,N )!=-1 ) 
      printf("I received data %s\n",buf);
   else
      perror("read error:");
    exit(EXIT_SUCCESS);  
}  
