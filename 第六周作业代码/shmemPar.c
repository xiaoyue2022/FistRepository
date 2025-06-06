// shmemPar.c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <error.h>
#define SIZE 1024
int main()
{
    int shmid ; //共享内存段标识符 
    char *shmaddr ;
    char buff[30];
    int shmstatus; //获取共享内存属性信息
    int pid ; //进程ID号
    shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT|0600 ) ;
    if ( shmid < 0 )    {
            perror("get shm  ipc_id error") ;
            return -1 ;
    }
    pid = fork() ;
    if ( pid == 0 )     {  //子进程向共享内存中写入数据
        shmaddr = (char *)shmat( shmid, NULL, 0 ) ; //映射共享内存，可读写
        if ( (int)shmaddr == -1 )        {
            perror("shmat addr error") ;
            return -1 ; 
        }
     strcpy( shmaddr, "Hello World!\n") ; 
       shmdt( shmaddr ) ;
      return  0;
    } else if ( pid > 0) {
     sleep(10);
        shmaddr = (char *) shmat(shmid, NULL, 0 ) ;
        if ( (int)shmaddr == -1 )        {
            perror("shmat addr error") ;
            return -1 ;
        }
   strcpy(buff,shmaddr);
   printf("I have  got from shared memory :%s\n", buff) ;
     shmdt( shmaddr ) ;
     shmctl(shmid, IPC_RMID, NULL) ;
    }else{
        perror("fork error") ;
        shmctl(shmid, IPC_RMID, NULL) ;
    } 
    return 0 ;
}
