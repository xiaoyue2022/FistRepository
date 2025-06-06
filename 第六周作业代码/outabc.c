#include <sys/types.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
char SEM_NAME[]= "process1";
char SEM_NAME2[]= "process2";
#define SHMSZ 27

int main(int argc,char **argv)
{
    char ch;
    int shmid;
    key_t key;
    int fd;
    sem_t *mutex,*mutex2;
    
    
    //create & initialize existing semaphore
    mutex = sem_open(SEM_NAME,O_CREAT,0777,0);    
    mutex2 = sem_open(SEM_NAME2,O_CREAT,0777,1);    
    
    if(mutex == SEM_FAILED||mutex2==SEM_FAILED)    {
        perror("unable to execute semaphore");
        sem_close(mutex);
        sem_close(mutex2);
        exit(-1);
    }
    fd=open("a.txt",O_CREAT|O_WRONLY|O_TRUNC|O_APPEND);
    if(fd == -1)    {
        perror("open failed");
        sem_close(mutex);
        exit(-1);
    }   
    while (1) {
         sem_wait(mutex);  //P²Ù×÷
         write(fd,"ABC\n",sizeof("ABC\n")-1);
         close(fd);         
         fd=open("a.txt",O_WRONLY|O_APPEND);
         sleep(1);
         sem_post(mutex2);       //V²Ù×÷
        }
    
    sem_close(mutex);
    sem_unlink(SEM_NAME);
    close(fd);
    
    exit(0);
}

	
	
	
	
	
	
	
	
	
	
	
       