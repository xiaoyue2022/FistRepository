/*first.c*/
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
typedef struct{
  char name[4];
  int  age;
}student;
main(int argc, char** argv) {
  int fd,i;
  student *p_map;
  char temp;
  fd=open("stu.txt",O_CREAT|O_RDWR|O_TRUNC,00777);
  lseek(fd,sizeof(student)*5-1,SEEK_SET);
  write(fd,"",1);  
  p_map = (student*) mmap( NULL,sizeof(student)*10,PROT_READ|PROT_WRITE,
        MAP_SHARED,fd,0 );
  if (p_map==(void *)-1){
     perror("mmap failed\n");
     exit(0);
   }
    
  close( fd );
  
  temp = 'a';
  for(i=0; i<10; i++)
  {
    temp += 1;  //学生姓名
    memcpy( ( *(p_map+i) ).name, &temp,2 );
    ( *(p_map+i) ).name[1]='\0';
    ( *(p_map+i) ).age = 20+i;  //学生年龄
  }
  sleep(10);
  munmap( p_map, sizeof(student)*10 );  
}
