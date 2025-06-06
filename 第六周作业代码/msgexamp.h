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
struct my_msg_st   //消息结构体
{
	long my_msg_type;  //消息类型，其值分别为1，2，3代表不同交易消息
	char  text[MAX_TEXT];  //存放具体消息内容
};
#endif
