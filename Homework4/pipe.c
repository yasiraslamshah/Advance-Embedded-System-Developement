//Author Yasir Aslam Shah
//Homework 4
//AESD Spring 2019
//Pipes

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/types.h>
#include<sys/time.h>
#include<time.h>
struct buffer
{
  char *string;
  int length;
};
FILE *fptr;
int main()
{
  int fd1[2];
  int fd2[2];
  struct buffer buffer1,buffer2,buffer3,buffer4;

  struct timespec start0,start1,stop0,stop1;
  
  buffer1.string="PAPA! I NEED MONEY!";
  buffer1.length=strlen(buffer1.string);

  buffer2.string="WORK AND EARN!";
  buffer2.length=strlen(buffer2.string);
  fptr=fopen("pipe_log.log","a");
  pid_t pipes;
  
  if(pipe(fd1)== -1)
  {
    fprintf(fptr,"Error:Failed to create Pipe\n");
    return 1;
  }
  if(pipe(fd2)== -1)
  {
    fprintf(fptr,"Error:Failed to create Pipe\n");
    return 1;
  }
  fclose(fptr);
  pipes =fork();
  if(pipes < 0)
  {  
    fptr=fopen("pipe_log.log","a");
    fprintf(fptr,"Error:Failed to FORK\n");
    printf("\nERROR: Fork failed!\n");
    fclose(fptr);
    return 1; 
  }
  else if(pipes ==0)
  { //child
    fptr=fopen("pipe_log.log","a");
    fprintf(fptr,"Entering Child Thread\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start0);
    fprintf(fptr,"\nChild Enter Timestamp:%ld\n",(start0.tv_sec*(long)1e9)+start0.tv_nsec);
    for(int y=0;y<10;y++)
    {
      close(fd1[1]);   //closing writing end of fd1
      read(fd1[0],&buffer3,sizeof(buffer3));
      close(fd1[0]);  //closing reading end of fd1
    
      printf("\nString recieved by Child from Parent : %s\t",buffer3.string);
      printf("String Length: %d\n",buffer3.length);
    
      fprintf(fptr,"String recieved by Child from Parent : %s\tString Length: %d\n",buffer3.string,buffer3.length);

      close(fd2[0]);   //closing reading end of fd2
      write(fd2[1],&buffer1,sizeof(buffer1));
      close(fd2[1]);
    }
      fclose(fptr);
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop0);
      fprintf(fptr,"\nChild Exit Timestamp:%ld\n",(stop0.tv_sec*(long)1e9)+stop0.tv_nsec);
    exit(0);
  }
  else
  { //parent
    fptr=fopen("pipe_log.log","a"); 
    fprintf(fptr,"Entering Parent Thread\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
    fprintf(fptr,"\nParent Enter Timestamp:%ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);

    for(int i=0;i<10;i++)
    {
      close(fd1[0]);  //closing reading end of fd1
      write(fd1[1],&buffer2,sizeof(buffer2));

      close(fd1[1]);  //closing writing end of fd1
      close(fd2[1]);  //closing writing end of fd2
      
      read(fd2[0],&buffer4,sizeof(buffer4));
      close(fd2[0]);
      printf("\nString recieved by Parent from Child: %s\t",buffer4.string);
      printf("String Length: %d\n",buffer4.length);
      fprintf(fptr,"String recieved by Parent from Child : %s\tString Length: %d\n",buffer4.string,buffer4.length);

      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop1);
      fprintf(fptr,"\nParent Exit Timestamp:%ld\n",(stop1.tv_sec*(long)1e9)+stop1.tv_nsec);
    }
      fclose(fptr);
  }
}





