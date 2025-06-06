/*semlib.c*/
#include "semlib.h"
int init_sem(int sem_id,int init_value)
{
	union semun sem_union;
//�����ź����ĳ�ʼֵ
	sem_union.val=init_value;  
//SETVAL������ʾ�����ź����ĳ�ʼֵ
	if(semctl(sem_id,0,SETVAL,sem_union) == -1)	{
		perror("initializing semaphore"); 
		return -1;
	}
	return 0;
}
/*��ϵͳ��ɾ���ź���*/
int del_sem(int sem_id)
{
	union semun sem_union;
// IPC_RMID ������ʾɾ��sem_id�ź���
	if(semctl(sem_id,0,IPC_RMID,sem_union)==-1) 	{
		perror("Delete semaphore failed");
		return -1;
	}
}

/*�ź���P�������� */
int sem_p(int sem_id)
{
	struct sembuf sem_b;
	sem_b.sem_num=0;  //�ź�����ţ������ź���ʱ������Ϊ0
	sem_b.sem_op= -1;  //����Ϊ-1��ʾ����P���� 
	sem_b.sem_flg=SEM_UNDO; //�������˳�ʱδ�ͷŸ��ź���ʱ���ɲ���ϵͳ�����ͷ�
// �Ա��Ϊsem_id���ź���ִ��P����
	if(semop(sem_id,&sem_b,1)==-1)	 {
		perror("P operation failed");
		return -1;
	}
	return 0;
}
/*V ��������*/
int sem_v(int sem_id){
	struct sembuf sem_b;
	sem_b.sem_num=0;  
	sem_b.sem_op=1; //1��ʾV����
	sem_b.sem_flg=SEM_UNDO;
	if(semop(sem_id,&sem_b,1)==-1)		{
		perror("V operation failed");
		return -1;
	}
	return 0;
}
