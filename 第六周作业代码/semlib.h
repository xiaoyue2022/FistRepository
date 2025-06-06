/* semlib.h */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define DELAY_TIME  3 
//定义信号量编号联合体
union semun{ 
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};
//信号量初始化函数
int init_sem(int sem_id,int init_value);
//信号量删除函数
int del_sem(int sem_id); 
//信号量的P操作函数
int sem_p(int sem_id); 
//信号量的V操作函数
int sem_v(int sem_id); 
