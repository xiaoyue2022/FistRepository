/* read/write from popen */
/*popenexam.c*/
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
void main() {
  FILE *readfp,*writefp;
  char buf[100];
  readfp=popen("ps aux","r"); //r��ʾ��ȡ�ܵ�����ִ�н��
  writefp=popen("grep init","w"); //w��ʾ�����д������ 
  while( fgets(buf,99,readfp)!=NULL ) {
      fputs(buf,writefp);   
}
//��popen��ȡ���ݲ�д�뵽����һ�������� 
  pclose(readfp);
  pclose(writefp);
}
