/*msgsnd.c文件随机发送交易消息程序*/
#include "msgexamp.h"
#include<stdlib.h>
int main(){
	int index = 1;
	struct my_msg_st some_data;
	int msgid;
	char buffer[BUFSIZ];
	msgid = msgget((key_t)MSG_KEY, IPC_CREAT|S_IRUSR|S_IWUSR);	if(msgid == -1)	{
		perror("create message queue failed ");
	 return -1;
	}
	srand((int)time(0));
	while(index<5)	 {
		printf("[%d]Enter some text: less than %d\n",msgid,MAX_TEXT);
		fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = rand()%3+1;  //随机值
		printf("my msg_type=%ld\n",some_data.my_msg_type);
		strcpy(some_data.text, buffer);  //随机数字字符串
		 if (msgsnd(msgid, (void *)&some_data, sizeof(some_data), 0) == -1) 
		{
	 		fprintf(stderr, "msgsnd failed\n");
	 		exit(-1);
		 }	
    index++;	 
 }
	exit(0);
}
