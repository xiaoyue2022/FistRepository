#include <stdlib.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <stdio.h>
 #include <fcntl.h>  
 #include <string.h>
 
 void debug(char *mess,char *param, int n) 
 {
    if (n==-1) {
      printf("Error occured: %s in %s\n",mess,param);
      exit(1);
    }       
  }

 main( int ac, char **av )
 {
	 int	fd, nwrite,oldpos;	
	 fd=open("A.log", O_WRONLY|O_APPEND );  // ���ļ�A����ȡ�ļ�
     debug("Cannot open","A.log",fd);	
     oldpos=lseek(fd,0,SEEK_END);//λ��ָ�붨λ���ļ�ĩβ
     debug("Lseek error","A.log",oldpos);
     sleep(10);
     nwrite=write(fd,"BBBBBBBBB",strlen("BBBBBBBBB"));
     debug("Write error","A.log",nwrite);	 	
     nwrite=write(fd,"BBBBBBBBB",strlen("BBBBBBBBB"));
     debug("Write error","A.log",nwrite);			
     close(fd);		
}
