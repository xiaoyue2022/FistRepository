#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include "semlib.h"  //ǰ���½ڵ��ź��������⺯��
#include <string.h>
#define BUFFER_SIZE  2048
int main()
{
	pid_t pid;
	int sem_id; //�ź���ID 
	int shmid;//�����ڴ��ID
	char *shm_addr=NULL; //�����ڴ��׵�ַ
	char buff[40]; //�ַ���
   int i=0;
	sem_id=semget(ftok(".",'a'),1,0666|IPC_CREAT);/*�ź���*/
	init_sem(sem_id,0);//�ź�����ʼ��
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
		  sem_p(sem_id); //P����		  
	   	  strcpy(buff,shm_addr);  //��ȡ����
		  printf("Child get data from shared-memory:%s\n",buff);
		  sem_v(sem_id); //V����
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
	else  	{ //������
	  	if((shm_addr=shmat(shmid,0,0))==(char *)-1)		{
			   perror("Parent shmat failed");
			   return -1;
		}
    		while(i<3)  {
   		printf("Please input some string:\n");
	  	fgets(buff,BUFFER_SIZE,stdin);
		strncpy(shm_addr,buff,strlen(buff));
		sem_v(sem_id);//V����
                i++;
        	sem_p(sem_id); //P����
		}
		if((shmdt(shm_addr))<0)		{
			perror("Parent:shmdt");
			exit(1);
		}		
		waitpid(pid,NULL,0);
	} 
	return 0;
}
