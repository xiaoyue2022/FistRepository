#include <limits.h>  // max path length
#include <stdlib.h>
#define PATH_MAX_GUESS 100

#ifdef PATH_MAX
static int pathmax=PATH_MAX/4;
#else
static int pathmax=0;
#endif



char * path_alloc(int *size) // allocated buffer for directory path
{
	char *ptr;
	if (pathmax==0)
	{
		errno = 0;
		if (( pathmax=pathconf("/",_PC_PATH_MAX))<0)
		{
			if (errno==0)
			{
				pathmax=PATH_MAX_GUESS;
			}
			else
				printf("pathconf error for _PC_PAHT_MAX");
		}
		else
			pathmax++;
	}
	if ( ( ptr=malloc(pathmax+1) ) ==NULL)
	{
		printf("malloc error!\n");
	}
	if (size!=NULL)
	{
		*size=pathmax+1;
	}
	return ptr;
}
