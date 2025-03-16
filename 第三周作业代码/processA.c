#include <stdlib.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <fcntl.h> 
 #include <stdio.h>
 #include <string.h>
 void debug(char *mess,char *param, int n)  //辅助调试函数
 {
    if ( n==-1 ){      
      printf("Error occured: %s %s\n",mess,param);
      exit(1);
    }       
  }

 main( int ac, char **av )
 {
	 int	fd, nwrite,oldpos;	
	 fd=open("A.log", O_WRONLY|O_APPEND );  // 打开文件A，读取文件
     debug("Cannot open","A.log",fd);	
     oldpos=lseek(fd,0,SEEK_END);//位置指针定位到文件末尾
     debug("Lseek error","A.log",oldpos); //这里的debug是上例中所使用的debug函数
     sleep(10);
     nwrite=write(fd,"AAAAAAAAAAAAA",strlen("AAAAAAAAAAAAA"));
     debug("Write error","A.log",nwrite);	 		
     nwrite=write(fd,"AAAAAAAAAAAAA",strlen("AAAAAAAAAAAAA"));
     debug("Write error","A.log",nwrite);	 			
	 close(fd);		
}
