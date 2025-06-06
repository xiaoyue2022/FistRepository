/*semexample.c  主程序，实现父子进程之间执行顺序的控制*/
#include "semlib.h"
int main(void)
{
	pid_t result;
	int sem_id;
    /*获取信号量的标识符，在下列中，不同独立进程可获取相同的信号量标识符，该操作通过ftok函数获得信号量的键值。ftok函数通过获取第一个参数所指定的文件的i节点号，在其之前加上子序号作为键值返回*/
	sem_id=semget(ftok(".",'a'),1,0666|IPC_CREAT);
	init_sem(sem_id,0);/*设置信号量的初值为0*/
	result=fork();
	if(result==-1)	{
		perror("Fork failed\n");
	}
	else if(result==0)/*子进程*/
	{
		
		sleep(DELAY_TIME);
		printf("The child progress output\n");
		printf("BBBBBBBBBBBB\n");
		sem_v(sem_id);	//V操作
	} 
	else {   /*父进程*/
		sem_p(sem_id);  //P操作
		printf("The father process output \n");
     		printf("AAAAAAAAAAA\n");
     		del_sem(sem_id);
		
	}
	
	exit(0);	
}
