#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdbool.h>
#include <time.h>

struct message
{
  char *string;
  int length;
};
FILE *fp;
int main()
{
  struct timespec start,stop,start1,stop1,start2,stop2;
  fp=fopen("msg_log.log","a");
  pid_t fork_thread;
  mqd_t messageQ;
  struct message bufferX;
  //printf("\nhi1\n");
  int max = sizeof(bufferX);
  printf("\nSize is %d\n",max);
 
  //mq_unlink("/msg_queue");
  
  fork_thread=fork();
  //printf("\nhi111\n");
  fprintf(fp,"\nMessage Q\n");
  if(fork_thread < 0)
  {
    printf("\nError:Fork Failed!\n");
    fprintf(fp,"\nMessage Q Fork Failed\n");
    return 0;
  }
  else if(fork_thread ==0)
  { 
    struct mq_attr attrib;
    bzero(&attrib, sizeof(attrib));	
    attrib.mq_flags = 0;
    attrib.mq_maxmsg = 10;
    attrib.mq_msgsize = 99;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    fprintf(fp,"Child Time stamp %ld\n",(start.tv_sec*(long)1e9+start.tv_nsec));
    fprintf(fp,"\nMessage Q Forked!\n");
    for(int d=0;d<10;d++)
    {
    messageQ = mq_open("/msg_queue", O_CREAT | O_RDWR, 0666, &attrib);
    if(messageQ<0)
    {
      printf("\nError: MessageQ Descriptor Failed\n");
      fprintf(fp,"\nError: MessageQ Descriptor failed\n");
	    exit(1);
    }
    char buffer[100];
    bzero(buffer,100);
    int res1 = mq_receive(messageQ,(char *)buffer, sizeof(buffer), 0);
    if(res1 <0)
    {
      printf("\nError1:Parent to Child Recieved Failed\n");
      fprintf(fp,"\nError1:Recieved Failed\n");
	    exit(1);
    }
    else printf("\nMessage recieved\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
    fprintf(fp,"Recieved Child Time stamp %ld\n",(start1.tv_sec*(long)1e9+start1.tv_nsec));
 
    fprintf(fp,"\nMessage Recieved from Parent\n");
    printf("\nString from Parent to Child : %s Length: %ld\n", buffer, sizeof(buffer));
    fprintf(fp,"\nMessage from Parent String %s,Length %ld\n",buffer,sizeof(buffer));

    char *buffer1="Hello_from_Child";
    int length1=strlen(buffer1);
    printf("\nString from Child: %s Length%d\n", buffer1, length1);

    int res2 = mq_send(messageQ, (char *) buffer1, length1, 0);
    if(res2 <0)
    {
      printf("\nError1:Child to Parent Send Failed\n");
      fprintf(fp,"\nError:Send Failed\n");
	    //exit(1);
    }
    else printf("\nMessgae Recieved!\n");

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop1);
    fprintf(fp,"Child send Time stamp %ld\n",(stop1.tv_sec*(long)1e9+stop1.tv_nsec));
    printf("\nChild sent string!\n");
    }
    mq_close(messageQ);
  }

  else
  {
      struct mq_attr attrib;
      bzero(&attrib, sizeof(attrib));	
      attrib.mq_flags = 0;
      attrib.mq_maxmsg = 10;
      attrib.mq_msgsize = 99;
for(int x=0;x<10;x++)
{
      messageQ = mq_open("/msg_queue", O_CREAT | O_RDWR, 0666, &attrib);

      if(messageQ<0)
      {
        printf("\nError2: MessageQ Descriptor Failed\n");
        fprintf(fp,"\nMessageQ Descriptor failed!\n");
	      //exit(1);
      } 
      char *buffer2 = "Bye_from_Parent";
      int length2 =strlen(buffer2);

      usleep(5000);
      fprintf(fp,"\nSleep");


      //printf("\nString from Parent: %s Length: %d \n",buffer2., buffer2.length);
      int res3 = mq_send(messageQ, (const char *) buffer2,length2, 0);
      if(res3 <0)
      {
        printf("\nError2:Parent to Child Send Failed\n");
        fprintf(fp,"\nError: Send Failed\n");

	      //exit(1);
	    } 
      else printf("\nMessage Sent\n");
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start2);
      fprintf(fp,"Sent Child Time stamp %ld\n",(start1.tv_sec*(long)1e9+start1.tv_nsec));
 

      bzero(&buffer2,sizeof(buffer2));
      char buffer4[100];
      bzero(&buffer4,100);
      int res4=mq_receive(messageQ,(char *)buffer4,sizeof(buffer4), 0);
      if(res4 <0)
      {
        printf("\nError2:Parent to Child Recieved Failed\n");
	      fprintf(fp,"\nError: Recieved Failed\n")
        ;//exit(1);
	    }
      else printf("\nMessage recieved\n");
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
      fprintf(fp,"Recieved Child Time stamp %ld\n",(stop2.tv_sec*(long)1e9+stop2.tv_nsec));
 
    
      printf("\nString Recieved by Parent : %s Length: %ld \n", buffer4,sizeof( buffer4));
      fprintf(fp,"\nString Recieved by Parent : %s Length: %ld \n", buffer4,sizeof( buffer4));
     }
      mq_close(messageQ);
      mq_unlink("/msg_queue");
      fprintf(fp,"\nMessage closed");
  }
fclose(fp);
 clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop);
    fprintf(fp,"Recieved Child Time stamp %ld\n",(stop.tv_sec*(long)1e9+stop.tv_nsec));
 

  exit(EXIT_SUCCESS);
}

