/*duptest1.c*/
 #include<unistd.h>
 #include<fcntl.h>
 #include<stdio.h>
 #include<stdlib.h>
 #include<sys/types.h>
 #include<errno.h>
 void main()
 {
    int newfd,oldfd1,oldfd2,newfd2,nchar;
    char buf[30];
    oldfd1=open("a.txt",O_RDWR);
    oldfd2=open("b.txt",O_RDWR);
   if (oldfd1>0) printf("The oldfd1 file descriptor =%d\n",oldfd1);
   if (oldfd2>0) printf("The oldfd2 file descriptor =%d\n",oldfd2);
   newfd=dup(oldfd1);
   printf("The newfd file descriptor =%d\n",newfd);
   newfd2=dup2(oldfd1,0);
   if (newfd2==-1) {
    perror("dup2 failed\n");
    exit(0);
    }
   printf("The newfd2 file descriptor =%d\n",newfd2); 
   nchar=read(newfd2,buf,28);
   if (nchar==-1)
   {
   	perror("read failed\n");
    	exit(0);   	
   	}
   buf[nchar]='\0';
   printf("I have read from a.txt:%s\n",buf);   
   
 }
