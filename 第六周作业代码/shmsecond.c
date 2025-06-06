/*-------------second.c----------*/
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
typedef struct{
  char name[4];
  int  age;
}student;
main(int argc, char** argv) 
{
  int fd,i;
  student *p_map;
  fd=open( "stu.txt",O_CREAT|O_RDWR,00777 );
  p_map = (student*)mmap(NULL,sizeof(student)*10,PROT_READ|PROT_WRITE,
       MAP_SHARED,fd,0);
  for(i = 0;i<10;i++)
  {
  printf( "name: %s age %d;\n",(*(p_map+i)).name, (*(p_map+i)).age );
  }
  munmap( p_map,sizeof(student)*10 );
}
