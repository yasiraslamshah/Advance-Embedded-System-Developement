//Author: Yasir Aslam Shah
//HW4
//AESD Spring2019

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
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
  fp=fopen("shared_log.log","a");
  struct timespec start,stop,start1,stop1,start2,stop2,startc,startp;
  pid_t Thread ;

  int shm_var;
  sem_t *sem_var;
  
  void *m_pointer = NULL;

  char *user = "/yasir_semaphore";
  char *semaphore_name= "semaphore";
  struct message buff; 
  int size =sizeof(buff);

  Thread =fork();
  if(Thread <0)
  {
    printf("\nError: Fork Creation Failed!\n");
    fprintf(fp,"\nError in Forking\n");
  }
  else if(Thread ==0)
  {
    fp=fopen("shared_log.log","a");
    shm_var = shm_open(user, O_CREAT | O_RDWR, 0666);
    sem_var = sem_open(semaphore_name, O_CREAT,0666, 0);
    fprintf(fp,"\nChild Process Created\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
    fprintf(fp,"\nChild Process Timestamp:\n");
    fprintf(fp,"\ %ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);

    if(shm_var < 0)
    {
	    perror("\nError: SHM_OPEN Failed\n");
	    fprintf(fp,"\nError:SHM_OPEN\n");
      exit(1);
   	}
          
    if(ftruncate(shm_var, size) == -1)
      {
	        perror("Error: Failed to Resize\n");
          fprintf(fp,"\nError:Resize\n");
	        exit(1);
      }

    m_pointer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_var, 0); 
    if (m_pointer == NULL)
    {
	    perror("Error:Mapping Failed!\n");
      fprintf(fp,"\nError:Memory Mapping\n");
	    exit(1);
    }

    if(sem_var < 0)
    {
      printf("\nError: Sem_Open Failed!\n");
      fprintf(fp,"\nError:Sem_Open\n");
    }

    sem_wait(sem_var);
    fprintf(fp,"\nSem_wait Issues\n");

    struct message buffer1;
for(int h=0;h<10;h++)
{
    memcpy((void *) &buffer1,m_pointer,size);
    printf("String Recieved by CHILD: No.%d %s, Length: %d\n",h, buffer1.string, buffer1.length);

    fprintf(fp,"String Recieved by CHILD:No.%d %s, Length: %d\n",h, buffer1.string, buffer1.length);
    struct message buffer2;
    buffer2.string ="Child says Hi!";
    buffer2.length =strlen(buffer2.string);
    printf("String Sent by CHILD %d: %s, Length: %d\n",h,buffer2.string,buffer2.length);
    fprintf(fp,"String Sent by CHILD %d: %s, Length: %d\n",h,buffer2.string,buffer2.length);
    fprintf(fp,"\nChild Sent Process\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&startc);
    fprintf(fp,"\tTimestamp:\n");
    fprintf(fp,"\ %ld\n",(startc.tv_sec*(long)1e9)+startc.tv_nsec);


    memcpy(m_pointer, (void *) &buffer2,size);    
    msync(m_pointer, size, MS_SYNC);
    
    sem_post(sem_var);
 }  fprintf(fp,"\nSem Post Issued\n");
    fprintf(fp,"CHILD process Exit\t");
    fprintf(fp,"Timestamp:");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop1);
    fprintf(fp," %ld\n",(stop1.tv_sec*(long)1e9)+stop1.tv_nsec);
    close(shm_var);
    sem_close(sem_var);
    sem_unlink("semaphore");
    fclose(fp);
  }
  else if(Thread >0)
  {
      fp=fopen("shared_log.log","a");
      fprintf(fp,"Parent Process\n");
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start2);
      fprintf(fp,"\nParent Process Timestamp:\n");
      fprintf(fp," %ld\n",(start2.tv_sec*(long)1e9)+start2.tv_nsec);

      shm_var = shm_open(user, O_CREAT | O_RDWR, 0666);
      sem_var = sem_open(semaphore_name, O_CREAT,06666, 0);

      if(shm_var < 0)
      {
	      printf("Error:Descriptor creation Failed!\n");
        fprintf(fp,"\nError:Descriptor Failed\n");
	      exit(1);
      }

      if(ftruncate(shm_var, size) == -1)
        {
	        perror("Error:Resize Failed\n"); 
          fprintf(fp,"\nError:Resize Mem\n");
	        exit(1);
        }

      m_pointer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_var, 0);
      if (m_pointer == NULL)
        {
	        printf("Error: Mapping Failed\n"); 
          fprintf(fp,"\nError:Memory Mapping\n");
	        exit(1);
        }

      struct message buffer3;
for(int y=0;y<10;y++)
{
      buffer3.string="Parent says BYE!";
      buffer3.length=strlen(buffer3.string);
      usleep(500);
      printf("String Sent by Parent %d: %s Length: %d\n",y,buffer3.string, buffer3.length);
      fprintf(fp,"String Sent by Parent %d: %s Length: %d\n",y,buffer3.string, buffer3.length);
      memcpy(m_pointer, (void *) &buffer3, size);
      msync(m_pointer, size, MS_SYNC);
      sem_post(sem_var);
      fprintf(fp,"\nSem Post\n");
      
      fprintf(fp,"\nParent Process Sent\n");
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&startp);
      fprintf(fp,"\tTimestamp:\n");
      fprintf(fp,"\ %ld\n",(startp.tv_sec*(long)1e9)+startp.tv_nsec);

      usleep(500);
      bzero(&buffer3, size);

      sem_wait(sem_var);
      fprintf(fp,"\nSem Wait\n");
      memcpy(&buffer3, m_pointer, size);

      printf("String Recieved by Parent %d: %s Length: %d.\n",y, buffer3.string, sizeof(buffer3.string));
      fprintf(fp,"String Recieved by Parent %d: %s Length: %d.\n",y, buffer3.string, sizeof(buffer3.string));
}
      fprintf(fp,"\nParent Process Closed\n");
      fprintf(fp,"Timestamp:");
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop2);
      fprintf(fp,"\ %ld\n",(stop2.tv_sec*(long)1e9)+stop2.tv_nsec);
      close(shm_var);
      sem_close(sem_var);
      sem_unlink(semaphore_name);
      shm_unlink(user);
      fclose(fp);
  }
  exit(EXIT_SUCCESS);
}
