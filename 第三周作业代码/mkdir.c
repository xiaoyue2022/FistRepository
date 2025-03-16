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

/*decide whether the path is directory or not*/

int isDir(char* path)
{
	struct stat *buf 	= (struct stat *)malloc(sizeof(struct stat *));
	mode_t t;
	//assert(path);
	if (stat(path,buf)==-1)
	{
		printf("open file properties failed:%s %s\n",strerror(errno),path);
		exit(0);
	}
	t=buf->st_mode;
	//free(buf);
	return S_ISDIR(t);
}

void freestack()
{
	while ( point)
	{
	//	free(stack[point]);
		stack[point]=0x0;
		point--;
	}
}
/*delete the director which is given
if it is not empty then recursivelly execute the proceduer
*/
int myrmdir(char *path)
{
	DIR *dir = NULL;
	struct dirent *pent = NULL;
	char *path1,*path2;
	int size; // path size
	int size2;
    char *temp;
 //   dir = ( DIR *)malloc(sizeof(DIR * ));	
//	pent = (struct dirent *)malloc(sizeof(struct dirent * ));
	path1 = path_alloc(&size);
	path2 = path_alloc(&size2);
	memset(path1,0,size);
	printf("size=%d\n",size);
	printf("size2=%d\n",size2);
	if ( isDir(path)  )
	{
		printf("path=%s\n",path);
		//printf("before chdir current path=%s\n",getcwd(path1,size));
		//printf("error:%s\n",strerror(errno));
		//chdir(path);
		//path1 = path_alloc(&size);
		//printf("after chdir current path=%s\n",getcwd(path1,size));
		push(path);
//		(char *)getcwd(path1,size)
		//chdir("..");
		//printf("after chdir current path=%s\n",getcwd(path1,size));*/
	} 
	/*strcat(parent,path);
	printf("parent=%s\n",parent);*/
//    temp = (char *)calloc(20,sizeof(char *));
	while(!isEmpty())
	{
		//strcpy(temp,(char *)pop());		
		strcpy(path1, pop());
		//printf("s=%s\n",s);
		//strcat(pop());
		/*if (chdir(temp)<0)
		{
			printf("chdir error:%s\n",strerror(errno));
			freestack();
			return 0;
		}*/
		
				
		//assert(!dir);
		if ( (dir = opendir(path1) )==NULL )
		{
			printf("error occured:%s %s\n",strerror(errno),temp);
			freestack();
			return 0;
		}
		strcpy(path2,path1);// backup path1
		while ( pent=readdir(dir) )
		{
			printf("pent->d_name=%s\n",pent->d_name);
			
			if (  !( ( strcmp(pent->d_name,".")==0 || strcmp(pent->d_name,"..")==0 ) ) )  
			{
				//strcat(temp,pent->d_name);
				
				strcat(path1,"/");
				strcat(path1,pent->d_name);
				if ( isDir( path1 ) )
				{
//					chdir(path1);
					push(path1);				
					path1=path_alloc(&size);
					
				}
				else  // delete file
				{
						        
								unlink(path1);
								printf("file:%s is deleted......\n",path1);
								//free(path1);
				}

			}
			memset(path1,0,sizeof(path1));
			strcpy(path1,path2);// restore path value
			//memset(path2,0,sizeof(path2));
		}
		free(path1);
		closedir(dir);
		//rmdir();
		
	}
	freestack();
	free(dir);
	free(pent);

}

int main(int argc,char **argv)
{
	assert(argc==2);
	/*if ( mkdir(argv[1],777)==-1 )
	{
		printf("%s",strerror(errno));
	
		}*/
    /*mymkdir(argv[1]);*/
	myrmdir(argv[1]);
	return 0;

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
