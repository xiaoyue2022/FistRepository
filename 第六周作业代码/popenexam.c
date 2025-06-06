/* read/write from popen */
/*popenexam.c*/
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
void main() {
  FILE *readfp,*writefp;
  char buf[100];
  readfp=popen("ps aux","r"); //r表示读取管道命令执行结果
  writefp=popen("grep init","w"); //w表示向进程写入数据 
  while( fgets(buf,99,readfp)!=NULL ) {
      fputs(buf,writefp);   
}
//从popen读取数据并写入到另外一个进程中 
  pclose(readfp);
  pclose(writefp);
}
