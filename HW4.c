//Author Yasir Aslam Shah
//Homework 4
//AESD Spring 2019

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/syscall.h>

struct thread_info
{
  pthread_t threadID;
  char *log_filename;
};

FILE *global_log;
int lookup[50];
int  flag;

static void *child1_function(void *str1);
static void *child2_function(void *str1);
void process_charac(char alpha);
void cpu_util(int signum);

pthread_t child1, child2;

void cpu_util(int signum)
{
  struct timespec start1;
  //printf("\ninside cpu util\n\n");
  if((signum == SIGUSR1)||(signum == SIGUSR2))
  {
    printf("\nExit:SIGUSR Signal!\n");
    pthread_exit(NULL);
  }
  else if(signum == SIGVTALRM)
  { 
    FILE *log1 =fopen("log1.log","a");
    if(!log1)
    {
      printf("\nError: Cant open LOG File!\n");
    }
    
    FILE *cpu_log =fopen("/proc/stat","r");
    if(!cpu_log)
    {
      printf("\nError: Cant open CPU_LOG File!\n");
      exit(0);
    }
    char charac1 =fgetc(cpu_log);
    while(charac1 != EOF)
    {
      fprintf(log1,"%c",charac1);
      charac1=fgetc(cpu_log);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
    fprintf(log1,"\nCPU Start at Time Stamp:\t");
    fprintf(log1,"%ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);
    printf("\nCPU\n");
    printf("Time %ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);
    
    //printf("\nexit cpu\n");
    flag == signum;
    fclose(cpu_log);
    fclose(log1);
  }
}


static void sig_func1(int a)
{
  printf("\ninside sig function\n");
  FILE *file1= fopen("log1.log","a");
  fprintf(file1,"\nExit Child 2\n");
  fclose(file1);
  flag = a;
}

static void *child2_function(void *str1)
{
  struct thread_info *str;
  str = (struct thread_info *)str1;
  char *filename=str->log_filename;
  char string[200];
  global_log =fopen(filename,"a");
  if(!global_log)
  {
    printf("\nError:Can't open LOG file!\n");
  }
  fprintf(global_log,"\n**********Thread2 Start**********\n");
  fprintf(global_log,"Thread PID: %d, Thread ID %d,POSIX Thread ID: %ld\n",getpid(),syscall(SYS_gettid),pthread_self());
  fclose(global_log);

  struct sigaction sig;
  struct itimerval timer;
  struct sigevent signal;
  memset(&sig,0,sizeof(sig));
  sig.sa_handler = &cpu_util;
  sigaction(SIGVTALRM,&sig,NULL);
  //sigaction(SIGUSR1,&sig,NULL);
  //sigaction(SIGUSR2,&sig,NULL);
  //printf("\nSig handler 1\n");
  
  timer.it_value.tv_sec =0;
  timer.it_value.tv_usec=100000;//100000;

  timer.it_interval.tv_usec =100000;//100000;
  timer.it_interval.tv_sec=0;
  //printf("\nsetting timer\n");
  setitimer(ITIMER_VIRTUAL,&timer,NULL);

  struct sigaction sig1;
  sig1.sa_handler =&sig_func1;
  sigaction(SIGUSR1,&sig1,NULL);
  sigaction(SIGUSR2,&sig1,NULL);
  
  //printf("\nbefore while1\n");

  while(1)
  {
    if(flag== SIGUSR1)
    {
      fprintf(global_log,"\nThread2 Exiting! by USR1 Signal\n");
      fprintf(global_log,"Thread2 PID: %d, Thread2 ID %d,POSIX Thread2 ID: %ld",getpid(),syscall(SYS_gettid),pthread_self());
      fclose(global_log);
      printf("\nExit: Child2, SIGNAL:USR1\n");
      pthread_exit(NULL);
    }
    else if(flag== SIGUSR2)
    {
      fprintf(global_log,"\nThread2 Exiting! by USR2 Signal\n");
      fprintf(global_log,"Thread2 PID: %d, Thread2 ID %d,POSIX Thread2 ID: %ld",getpid(),syscall(SYS_gettid),pthread_self());
      fclose(global_log);
      printf("\nExit: Child2, SIGNAL:USR2\n");
      pthread_exit(NULL);
    }
  }
}

static void *child1_function(void *arg)
{  
  //this function reads the text file and computes the 
  //number of chars an logs them in log file
  printf("\nREACHED HERE\n") ;
  struct timespec start0,stop1,start1;
  FILE *reader;
  struct thread_info *str = (struct thread_info *)arg;
  printf("file name is: %s\n",str->log_filename);
  char *filename=str->log_filename;
  printf("FILE NAME IS:%s\n",filename);
  int res=1;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
  clock_gettime(CLOCK_MONOTONIC,&start0);
  reader =fopen("text","r");
  if(reader == NULL)
  {
    printf("Error:Cant Read File!");
  }
  else printf("\nFile read successfully!\n");
  char charac='1';
  while(!feof(reader))
  {
    char charac = fgetc(reader);
    printf("\n %c\t",charac);
    process_charac(charac);
  }
  printf("\nhello\n");
  fclose(reader);

  global_log =fopen(filename,"a");
  if(!global_log)
  {
    printf("\nError:Cant open LOG file\n");
  }
  //printf("\nhi\n");
  fprintf(global_log,"\nThread1 Start at Time Stamp:\t");
  fprintf(global_log,"%ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);
  printf("\nhi2\n");
  int k=0;
  while(k<26)
  {
    if(lookup[k]<100)
    {
      fprintf(global_log,"%c Occurs %d Times\n",(k+97),lookup[k]);
      printf("\n%c Occurs %d Times\n",(k+97),lookup[k]);
    
    }
    k++;
  }
  //printf("\nhi3\n");
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop1);
  fprintf(global_log,"\nThread1 Exiting Time Stamp at\t");
  fprintf(global_log,"%ld\n",(stop1.tv_sec*(long)1e9)+stop1.tv_nsec);
  fclose(global_log);
  printf("\nExit CHILD1\n");
  pthread_exit(NULL);
}

void process_charac(char alpha)
{
    char temp =alpha;
    char alpha2 =alpha;
    //lowercasing the char from file
    if((alpha2 >= 'A')&&(alpha2 <='Z'))
    {
      alpha2= alpha2+32;
    }
    if(alpha2 >='a' && alpha2 <='z')
    {
      printf("\nchar value is:%c\n",alpha2);
      lookup[alpha2-97]++;
      printf("Lookup[%d]: %d, %d",(alpha2-97),lookup[alpha2-97]);
    }
} 

int main(int argc, char *argv[])
{
  //thread creation 
  struct thread_info *p_info;
  struct timespec start,stop;
  p_info=(struct thread_info*)malloc(sizeof(struct thread_info));
  p_info->log_filename =argv[1];
  global_log = fopen(p_info->log_filename,"a");
  int res1,res2;

  res1= pthread_create(&child1,NULL,child1_function,(void*)p_info);
  if(res1)
  {
    printf("\nError:Thread Creation for Child1 FAILED!\n");
    FILE *global_log =fopen(p_info->log_filename,"a");
    fprintf(global_log,"Parent ID:%d PThread1 ID:%d,Linux1 ID:%d",getpid(),getpid(),syscall(SYS_gettid));
    fprintf(global_log,"%s","Child1 Thread Failed");
    fclose(global_log);
  }
  else
  {
    FILE *global_log =fopen(p_info->log_filename,"a");
    printf("\nChild 1 Created\n");
    fprintf(global_log,"\nChild1 Thread Created\n");
    fclose(global_log);
  }
  pthread_join(child1,NULL);  

  res2= pthread_create(&child2,NULL,child2_function,(void*)p_info);
  if(res2)
  {
    
    FILE *global_log =fopen(p_info->log_filename,"a");
    printf("\nError:Thread Creation for Child2 FAILED!\n");
    fprintf(global_log,"Parent ID:%d PThread2 ID:%d,Linux2 ID:%d",getpid(),getpid(),syscall(SYS_gettid));
    fprintf(global_log,"%s","Child2 Thread Failed");
    fclose(global_log);
  }
  else
  {
    printf("\nChild 2 Created\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    FILE *global_log =fopen(p_info->log_filename,"a");
    fprintf(global_log,"\nChild2 Thread Created at Time Stamp:\t");
    fprintf(global_log,"%ld\n",(start.tv_sec*(long)1e9)+start.tv_nsec);
    fclose(global_log);
  }
  if(pthread_join(child2,NULL))
  {
    exit(1);
  }
  else
  {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop);
    FILE *global_log =fopen(p_info->log_filename,"a");
    fprintf(global_log,"\nChild2 Thread Exit at Time Stamp:\t");
    fprintf(global_log,"%ld\n",(stop.tv_sec*(long)1e9)+stop.tv_nsec);
    fclose(global_log);
  }

    FILE *global_log =fopen(p_info->log_filename,"a");
    fprintf(global_log,"\nParent Thread Exit\n");
    fclose(global_log);
}
  
