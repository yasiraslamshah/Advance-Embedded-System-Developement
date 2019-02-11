#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include <time.h>
#include <sys/time.h>
//#include<linux/sortingyas.h>

void populate(int32_t *a,int32_t b);

int main()
{

	int32_t *buffer1 =NULL;
	int32_t *buffer2 =NULL;
	int32_t sizeofbuffer=256;

	//allocating buffer1 for unsorted array 
	buffer1 =(int32_t*)malloc(sizeofbuffer*sizeof(int32_t));
	if(buffer1 == NULL)
	{
		printf("\nError:Memory allocation failed\n");
	}
	printf("\nhi1\n");
	//allocating buffer2 to store sorted array
	buffer2= (int32_t*)malloc(sizeof(int32_t)*sizeofbuffer);
	if(buffer2 == NULL)
	{
		printf("\nError:Memory allocation failed\n");
	}

	//populate unsorted elements with rand function generator
	populate(buffer1,sizeofbuffer);

	sleep(1);

	//calling syscall sorting function
	int res =syscall(398,buffer1,buffer2,sizeofbuffer);
	if(res != 0)
	{
		printf("\nError: Syscal calling error\n");
	}

	sleep(1);
	//printing sorted elements
	for(int i=0;i<sizeofbuffer-1;i++)
	{
		printf("S: %d\t",*(buffer2+i));	
	}	
	printf("\n");
	//freeing dyncamic memory
	free(buffer1);
	free(buffer2);
return 0;
}


//function to populate using rand() function
void populate(int32_t *buffer1,int32_t length)
{
	for(int i=0; i<length; i++)
	{
		*(buffer1+i)= rand()%1000;
		printf("US: %d\t",*(buffer1+i));
	}
	printf("\n");
}
