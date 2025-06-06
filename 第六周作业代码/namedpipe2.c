/*namedpipe2.c*/  
#include<stdio.h>  
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <fcntl.h>
#define N 20
int main(){
   char buf[N];
   int fd= open("/home/lyq/mypipe",O_WRONLY);
   if(fd!=-1) {  
        printf("FIFO file is opened\n");  
    }  
    else    {
       perror("open failed"); 
       exit(0);
   }
printf("please input string\n");
scanf("%s",buf); getchar();
if ( write(fd,buf,sizeof(buf))!=-1 ) 
   printf("write successful\n");
else perror("write failed:");
   exit(EXIT_SUCCESS);  
}
