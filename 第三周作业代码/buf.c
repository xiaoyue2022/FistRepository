#include <stdio.h> 
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <fcntl.h>
 #define	BUFFERSIZE	10240
 void debug(char *mess,char *param, int n)  //辅助调试函数
 {
    if ( n==-1 ){      
      printf("Error occured: %s %s\n",mess,param);
      exit(1);
    }       
  }
 main( int ac, char **av )
 {
	int	in_fd, out_fd, n_chars;
	char	buf[BUFFERSIZE];  //存放从源文件读取的数据，每次读取BUFFERSIZE大小字节的数据
	if ( ac != 3 ) {  //根据输入参数长度判断是否输入源文件及目标文件名
		printf( "usage: %s source destination\n", *av);
		exit(1);
	}						
	 in_fd=open(av[1], O_RDONLY );  // 打开源文件，读取文件
     debug("Cannot open",av[1],in_fd);	
	 out_fd=creat( av[2], 0744 ); //创建目标文件，名称由参数av[2]决定
     debug("Cannot creat",av[2],out_fd);  						
	 while ( (n_chars = read( in_fd , buf, BUFFERSIZE )) > 0 )  //从源文件中读取，
		if ( write( out_fd, buf, n_chars ) != n_chars )//将读取的内容写入目标文件中
			debug("Write error to ", av[2],-1);						
	 close(in_fd);
     close(out_fd);//关闭文件		
}
