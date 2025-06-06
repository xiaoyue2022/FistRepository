/* semlib.h */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define DELAY_TIME  3 
//�����ź������������
union semun{ 
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};
//�ź�����ʼ������
int init_sem(int sem_id,int init_value);
//�ź���ɾ������
int del_sem(int sem_id); 
//�ź�����P��������
int sem_p(int sem_id); 
//�ź�����V��������
int sem_v(int sem_id); 
