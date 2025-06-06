/*semexample.c  ������ʵ�ָ��ӽ���֮��ִ��˳��Ŀ���*/
#include "semlib.h"
int main(void)
{
	pid_t result;
	int sem_id;
    /*��ȡ�ź����ı�ʶ�����������У���ͬ�������̿ɻ�ȡ��ͬ���ź�����ʶ�����ò���ͨ��ftok��������ź����ļ�ֵ��ftok����ͨ����ȡ��һ��������ָ�����ļ���i�ڵ�ţ�����֮ǰ�����������Ϊ��ֵ����*/
	sem_id=semget(ftok(".",'a'),1,0666|IPC_CREAT);
	init_sem(sem_id,0);/*�����ź����ĳ�ֵΪ0*/
	result=fork();
	if(result==-1)	{
		perror("Fork failed\n");
	}
	else if(result==0)/*�ӽ���*/
	{
		
		sleep(DELAY_TIME);
		printf("The child progress output\n");
		printf("BBBBBBBBBBBB\n");
		sem_v(sem_id);	//V����
	} 
	else {   /*������*/
		sem_p(sem_id);  //P����
		printf("The father process output \n");
     		printf("AAAAAAAAAAA\n");
     		del_sem(sem_id);
		
	}
	
	exit(0);	
}
