#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
int main()
{
   char buf[30]="123455678900000000";
   int fd=open("book.dat",O_RDWR);
    if (fd<0) {

        perror("open failed\n");
        exit(0);
    }
    if ( write(fd,buf,strlen(buf))<0 ){
       perror("write failed\n");
       exit(0);
	}
       

       

}

