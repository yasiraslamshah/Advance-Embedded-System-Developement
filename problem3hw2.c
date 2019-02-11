#include <stdio.h>
#define size 50
 void main()
{
	//local variables
	char ch;
	char string[size];

	printf("\nJoey and Chandler\n");	//printing string
	
	FILE *file1;				//creating a file
	file1=fopen("problem2.txt","r+");	//read 
	file1=fopen("problem2.txt","w+");	//write
	
	char var1='a';
	fputc( var1,file1);			//writing a character to file
        fclose(file1);

	file1=fopen("problem2.txt","a+");	//append mode
	dy_mem=(char *)malloc(sizeof(char));	//dynamically allocating
	printf("\nString:\n");			//read an input from command line and 
						//write to the string to allocated array
	
	scanf("%s",dy_mem);
	fprintf(file1,"%s",dy_mem);		//write string to the file
		
	//flush file output
	fclose(file1);

	file1=fopen("problem2.txt","r+");
	getc(ch);				//read a single char
	gets(string);				//read a string of char
	
	fclose(file1);
	free(dy_mem);				//free the memory










}
