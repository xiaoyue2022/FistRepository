//#include <sys/stat.h>  // for mkdir chdir
#include <unistd.h>
#include <stdio.h>
#include <errno.h>  // for strerror 
#include <assert.h>
#include <string.h> //for strtok;
#include <dirent.h> //for dir read 
#include <sys/stat.h>
#include <sys/types.h> // for macro S_ISDIR;
#include <stdlib.h>
#include "pathallocate.h"
#define MAXDIR 1000

char *stack[MAXDIR]; //   buffer for dir
int point = 0; //  point to the stack top;point to the next position for storing data
int flag=-1;

void push(char *name)
{
	if (!isFull())
	{
		stack[point++]=name;
	}


}
char *pop()
{
	if (!isEmpty())
	{
		return (char *)stack[--point];
	}
	else
	    return NULL;
}

int isEmpty()
{
	return (point==0?1:0);
}
int isFull()
{
	return (point==MAXDIR?1:0);
}

/* make a directory whose name is given by parameter 
0 successfully
1 failed
*/
int mymkdir(char *);

/*decide whether the path is directory or not
if path is a directory then return whether it is empty . 0 :empty 1:not empty
if path is a regular file then return 2;
else return -1;
*/


int isDir(char* path)
{
	struct stat *buf 	= (struct stat *)malloc(sizeof(struct stat *));
		
//	pent = (struct dirent *)malloc(sizeof(struct dirent * ));
	DIR* dir  = ( DIR *)malloc(sizeof(DIR * ));
	struct dirent *dire =(struct dirent *)malloc(sizeof(struct dirent * ));
	int count = 0;
	mode_t t;
	//assert(path);
	if (stat(path,buf)==-1)
	{
		printf("open file properties failed:%s %s\n",strerror(errno),path);
		return -1;
	}
	t=buf->st_mode;
	
	if (S_ISDIR(t))
	{
		
		dir = opendir(path);
	    if (dir==NULL)
	    {
			printf("opendir error:%s\n",strerror(errno));
			free(buf);
			return -1;
	    }
		while (( dire=readdir(dir) )!=NULL )
		{
			printf("dire->d_name:%s\n",dire->d_name);
			if (  !( ( strcmp(dire->d_name,".")==0 )|| (strcmp(dire->d_name,"..") ==0 ) ) )
			{
				count++;
			}
		}
		free(buf);
		closedir(dir);
		return (count>0)?1:0;

	}
	else
	{
		count ==2;
	}
	free(buf);
	free(dir);
	free(dire);
	return S_ISDIR(t);
}


void freestack()
{
	while ( point)
	{
		free(stack[point]);
		stack[point]=0x0;
		point--;
	}
}

/*delete the director which is given
if it is not empty then recursivelly execute the proceduer
*/

int myrmdir(char *path)
{
	DIR* dir  = ( DIR *)malloc(sizeof(DIR * ));
	static int i=1;
	i++;
	printf("i=%d\n",i);
	struct dirent *dire =(struct dirent *)malloc(sizeof(struct dirent * ));
	flag = isDir(path);
	if ( flag==1 )
	{
		//chdir(path);
		dir = opendir(path);
	    if (dir==NULL)
	    {
			printf("opendir error:%s\n",strerror(errno));			
			return -1;
	    }
		while (dir && ( dire=readdir(dir) ))
		{
			//printf("dire->d_name:%s\n",dire->d_name);
			if (  !( ( strcmp(dire->d_name,".") || strcmp(dire->d_name,"..") ) )  )
			{
				
					myrmdir(dire->d_name);				
			}
		}
		closedir(dir);

	}
	else if(flag==0)
	{
		chdir("..");
		printf("i am delete directory:%s\n",path);
		unlink(path);
	}
	else if ( flag==2 )
	{
		printf("i am deleting %s  file\n",path);
		unlink(path);
		return 0;

	}
	
	else
	{
		printf("error occured");
		return -1;
	}
	free(dir);
	free(dire);
	return 0;

}

int main(int argc,char **argv)
{
	assert(argc==2);
	/*if ( mkdir(argv[1],777)==-1 )
	{
		printf("%s",strerror(errno));
	
		}*/
    /*mymkdir(argv[1]);*/
	return myrmdir(argv[1]);


}

int mymkdir(char *name)
{
	char *s;
	int i=-1;
	assert( name != NULL );
	s=strtok(name,"/");
	while (s)
	{
	   if ( (i=mkdir(s,0775))==-1 )

	    {
		printf("%s\n",strerror(errno));
		return i;
  	    }
           else
	   {	i=chdir(s);}
	
        s= strtok(NULL,"/");   
	}
	return i;
}
