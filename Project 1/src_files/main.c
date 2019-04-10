/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : main.c
* Creation Date : 18-03-2019
* Last Modified : Sun 31 Mar 2019 00:9:56 PM MDT
* Created By : Harsimransingh
* Description: Source file to create all needed pthreads and implement the heartbeat functionality
*
* References
*           -https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/
*
_._._._._._._._._._._._._._._._._._._._._.*/


/*User defined libraries*/
#include "logger.h"
#include "light.h"
#include "temperature.h"
#include "socket.h"
#include "user_led.h"
#include "intial_test.h"
void intHandler(int dummy) 
{
  printf("\nSIGNAL HANDLER CAUGHT\n");
  exit_flag = 1;
}

void main(int argc, char *argv[])
{
  signal(SIGINT, intHandler);
  if(argc < 2)
  {
    printf("\nPlease input the log file name\n");
    exit(0);
  }
  pthread_t light_task, temperature_task, logging_task, socket_task;
  
  /*Mutex lock for synchronized access of logger function*/
  if(pthread_mutex_init(&logger_mutex,NULL)!= 0)
  {
    printf("\nlogger mutex creation failed\n");
  }
  /*Mutex lock for I2C bus synchronisation*/
  if(pthread_mutex_init(&bus_lock,NULL)!= 0)
  {
    printf("\nBus lock mutex creation failed\n");

  }
  /*Mutex lock for heartbeat pthread_cond_timedwait call*/
  if(pthread_mutex_init(&heartbeat_mutex,NULL)!=0)
  {
    printf("\nheartbeat mutex intialization failed\n");
    return;
  }
  /*conditional variable to implement heartbeat*/
  if(pthread_cond_init(&temp_thread_cond,NULL)!=0)
  {
    printf("\ntemp Condition variable creation failed\n");
    return;
  }
  /*conditional variable to implement heartbeat*/
  if(pthread_cond_init(&light_thread_cond,NULL)!=0)
  {
    printf("\ntemp Condition variable creation failed\n");
    return;
  }
  if(pthread_cond_init(&logger_thread_cond,NULL)!=0)
  {
    printf("\ntemp Condition variable creation failed\n");
    return;
  }
 
  thread_struct *thread_input = (thread_struct *)malloc(sizeof(struct thread_content));
  thread_input->log_file = argv[1];
  int result = intial_test(thread_input->log_file);
  if(result == EXIT_SUCCESS)
  {
    /*create light sensor thread*/
    if(!pthread_create(&light_task, NULL, light_function, (void *)thread_input))
    {
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Thread info: MAIN\nLIGHT Thread created Successfully\nLOG level:INFO",NULL);
    }
    else
    {
      printf("LIGHT read Thread creation failed\n");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"LIGHT read Thread creation failed",NULL);
    }

    /*create temperature sensor thread*/
    if(!pthread_create (&temperature_task, NULL, temperature_function, (void*)thread_input))
    {
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Thread info: MAIN\ntemperature thread created successfully\nLOG level:INFO",NULL);
    }
    else
    {
      printf("temperature Thread creation failed\n");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"temperature Thread creation failed",NULL);
    }

    /*create logging task thread*/
    if(!pthread_create (&logging_task, NULL, logging_thread, (void*)thread_input))
    {
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Thread info: MAIN\nlogging thread created successfully\nLOG level:INFO",NULL);
    }
    else
    {
      printf("logging Thread creation failed\n");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"logging Thread creation failed",NULL);
    }

    /*create socket task thread*/
    if(!pthread_create (&socket_task, NULL, socket_function, (void*)thread_input))
    {
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Thread info: MAIN\nsocket thread created successfully\nLOG level:INFO",NULL);
    }
    else
    {
      printf("socket Thread creation failed\n");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"socket Thread creation failed",NULL);
    }
    int return_value = 0;
    while(exit_flag != 1)
    {
      /*Toggle led to indicate prgram is still running*/
      static int light_led_status;
      led_control(BLUE,light_led_status);
      light_led_status ^=1;

      /*heartbeat timer setup*/
      struct timespec ts;
      ts = timer_setup(4,4000000);

      /*to check if temperature thread is alive*/
      pthread_mutex_lock(&heartbeat_mutex);
      return_value = pthread_cond_timedwait(&temp_thread_cond,&heartbeat_mutex,&ts);
      pthread_mutex_unlock(&heartbeat_mutex);
      if(return_value != 0 && exit_flag != 1)
      {
        intHandler(3);
        msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
        memset(msg->thread_name,'\0',sizeof(msg->thread_name));
        memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
        memset(msg->level,'\0',sizeof(msg->level));
        memcpy(msg->level,"ALERT",strlen("ALERT"));
        msg->unit = '\0';
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Temperature thread stuck",msg);
        break;
      }

      /*to check is light thread is alive or not*/
      pthread_mutex_lock(&heartbeat_mutex);
      return_value = pthread_cond_timedwait(&light_thread_cond,&heartbeat_mutex,&ts);
      pthread_mutex_unlock(&heartbeat_mutex);
      if(return_value != 0 && exit_flag != 1)
      {
        intHandler(3);
        msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
        memset(msg->thread_name,'\0',sizeof(msg->thread_name));
        memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
        memset(msg->level,'\0',sizeof(msg->level));
        memcpy(msg->level,"ALERT",strlen("ALERT"));
        msg->unit = '\0';
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"light thread stuck",msg);
        break;
      }
      pthread_mutex_lock(&heartbeat_mutex);
      return_value = pthread_cond_timedwait(&logger_thread_cond,&heartbeat_mutex,&ts);
      pthread_mutex_unlock(&heartbeat_mutex);
      if(return_value != 0 && exit_flag != 1)
      {
        intHandler(3);
        msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
        memset(msg->thread_name,'\0',sizeof(msg->thread_name));
        memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
        memset(msg->level,'\0',sizeof(msg->level));
        memcpy(msg->level,"ALERT",strlen("ALERT"));
        msg->unit = '\0';
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"logger thread stuck",msg);
        break;
      }
    }

    printf("\nREACHED HERE\n");
    /*join all the thread in completion*/
    pthread_join(temperature_task, NULL);
    pthread_join(light_task, NULL);
    pthread_join(logging_task,NULL);
  }
  printf("\nDestroy all\n");
  if(pthread_mutex_destroy(&heartbeat_mutex)!=0)
  {
    printf("\nheartbeat_mutex destroy: FAILED\n");
  }
  if(pthread_mutex_destroy(&bus_lock) != 0)
  {
    printf("\nbus_lock destroy: FAILED\n");
  }
  if(pthread_mutex_destroy(&logger_mutex) != 0)
  {
    printf("\nlogger_mutex destroy: FAILED\n");
  }
  if(pthread_cond_destroy(&temp_thread_cond)!=0)
  {
    printf("\ntemp_thread_cond destroy: FAILED\n");
  }
  if(pthread_cond_destroy(&light_thread_cond) != 0)
  {
    printf("\nlight_thread_cond destroy: FAILED\n");
  } 
}

