/*semlib.c*/
#include "semlib.h"
int init_sem(int sem_id,int init_value)
{
	union semun sem_union;
//设置信号量的初始值
	sem_union.val=init_value;  
//SETVAL参数表示设置信号量的初始值
	if(semctl(sem_id,0,SETVAL,sem_union) == -1)	{
		perror("initializing semaphore"); 
		return -1;
	}
	return 0;
}
/*从系统中删除信号量*/
int del_sem(int sem_id)
{
	union semun sem_union;
// IPC_RMID 参数表示删除sem_id信号量
	if(semctl(sem_id,0,IPC_RMID,sem_union)==-1) 	{
		perror("Delete semaphore failed");
		return -1;
	}
}

/*信号量P操作函数 */
int sem_p(int sem_id)
{
	struct sembuf sem_b;
	sem_b.sem_num=0;  //信号量编号，单个信号量时，设置为0
	sem_b.sem_op= -1;  //设置为-1表示进行P操作 
	sem_b.sem_flg=SEM_UNDO; //当程序退出时未释放该信号量时，由操作系统负责释放
// 对编号为sem_id的信号量执行P操作
	if(semop(sem_id,&sem_b,1)==-1)	 {
		perror("P operation failed");
		return -1;
	}
	return 0;
}
/*V 操作函数*/
int sem_v(int sem_id){
	struct sembuf sem_b;
	sem_b.sem_num=0;  
	sem_b.sem_op=1; //1表示V操作
	sem_b.sem_flg=SEM_UNDO;
	if(semop(sem_id,&sem_b,1)==-1)		{
		perror("V operation failed");
		return -1;
	}
	return 0;
}
