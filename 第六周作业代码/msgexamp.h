/*msgexamp.h*/
#ifndef MSGQUE_EXAMP
#define MSGQUE_EXAMP
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/stat.h>
#define MAX_TEXT 512
#define MSG_KEY 335
struct my_msg_st   //��Ϣ�ṹ��
{
	long my_msg_type;  //��Ϣ���ͣ���ֵ�ֱ�Ϊ1��2��3����ͬ������Ϣ
	char  text[MAX_TEXT];  //��ž�����Ϣ����
};
#endif
