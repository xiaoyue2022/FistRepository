/*msgrcv.c 接收指定类型消息的进程，头文件与上个程序一样 */
#include "msgexamp.h"
int main(int argc,char **argv) {
	int msgid;
	int type;
   struct my_msg_st *my_data=(struct my_msg_st *)malloc(sizeof(struct my_msg_st));
   
   if (argc<2) {
    printf("USAGE msgexample msgtype\n");
    return -1 ;
   }
   type=atoi(argv[1]);
   if ( type<0 || type>3 ) {
    printf("msgtype should be one of 1,2,3");
    return -1;
   }
  msgid = msgget((key_t)MSG_KEY, IPC_CREAT|S_IRUSR|S_IWUSR);
  if(msgid == -1)	{
		 perror("get message queue failed ");
	  return -1;
	}
	
    while (1) {	
 	  if (msgrcv(msgid,(void *)my_data, sizeof(struct my_msg_st),(long)type,IPC_NOWAIT) !=-1) {
      		 
		
        	printf("The message type is:%ld\n",my_data->my_msg_type);
        	printf("The message content is:%s\n",my_data->text);
           }	 
       else if (ENOMSG==errno)	{
	printf("there is no any message which is matched to message type \n");
	break;
   }
}

}