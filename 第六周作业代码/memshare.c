#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include "semlib.h"  //前面章节的信号量操作库函数
#include <string.h>
#define BUFFER_SIZE  2048
int main()
{
	pid_t pid;
	int sem_id; //信号量ID 
	int shmid;//共享内存段ID
	char *shm_addr=NULL; //共享内存首地址
	char buff[40]; //字符串
   int i=0;
	sem_id=semget(ftok(".",'a'),1,0666|IPC_CREAT);/*信号量*/
	init_sem(sem_id,0);//信号量初始化
	if((shmid=shmget(IPC_PRIVATE,BUFFER_SIZE,0666))<0)	{
		perror("shmget");
		return -1;
	}	
	pid=fork();
	if(pid==-1)	{
		perror("fork");
		return -1;
	}
	else if(pid==0) {
		if ((shm_addr=shmat(shmid,0,0))==(char *)-1) {
			perror("shmat");
			return -1;
		}
    		while ( i<3 ) {
		  
		  printf("Child process is waiting for data:\n");
		  sem_p(sem_id); //P操作		  
	   	  strcpy(buff,shm_addr);  //读取数据
		  printf("Child get data from shared-memory:%s\n",buff);
		  sem_v(sem_id); //V操作
      		  i++;
     }
		 del_sem(sem_id); //
	 	if((shmdt(shm_addr))<0) {
			perror("shmdt");
			return -1;
		}
		if(shmctl(shmid,IPC_RMID,NULL)==-1)		{
			perror("child process delete shared memory ");
			return -1;
		}	
	}  
	else  	{ //父进程
	  	if((shm_addr=shmat(shmid,0,0))==(char *)-1)		{
			   perror("Parent shmat failed");
			   return -1;
		}
    		while(i<3)  {
   		printf("Please input some string:\n");
	  	fgets(buff,BUFFER_SIZE,stdin);
		strncpy(shm_addr,buff,strlen(buff));
		sem_v(sem_id);//V操作
                i++;
        	sem_p(sem_id); //P操作
		}
		if((shmdt(shm_addr))<0)		{
			perror("Parent:shmdt");
			exit(1);
		}		
		waitpid(pid,NULL,0);
	} 
	return 0;
}
