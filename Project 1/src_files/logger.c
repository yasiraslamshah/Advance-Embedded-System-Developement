#include "logger.h"

struct timespec timer_setup(uint32_t second_value, uint32_t nanosec_value)
{
  struct timespec ts;
  struct timeval tp;

  gettimeofday(&tp,NULL);
  ts.tv_sec = tp.tv_sec;
  ts.tv_nsec = tp.tv_usec * 1000;
  ts.tv_sec += second_value;
  ts.tv_nsec += nanosec_value;
  
  return ts;
}

void *logging_thread(void *arg)
{
  thread_struct *logging_thread = (thread_struct *)arg;
  /*light queue attribute settings*/
  mqd_t light_discriptor;
  struct mq_attr attribute;
  attribute.mq_maxmsg = 10;
  attribute.mq_msgsize = sizeof(msg_struct);
  light_discriptor = mq_open ("/light_queue", O_RDWR | O_CREAT, 0666, &attribute);
  if (light_discriptor < 0)
  {
    printf("message queue error\n");
  }
  else
  {
    printf("Successful in creating message queue\n");
  }

  struct mq_attr temp_attribute;
  temp_attribute.mq_maxmsg = 10;
  temp_attribute.mq_msgsize = sizeof(msg_struct);
  /*temperature queue attribute setting*/
  mqd_t temperature_discriptor;
  temperature_discriptor = mq_open ("/temp_queue", O_RDWR | O_CREAT, 0666, &temp_attribute);
  if (temperature_discriptor < 0)
  {
    printf("message queue error\n");
  }
  else
  {
    printf("Successful in creating temp message queue\n");
  }
  while(exit_flag!=1)
  {
    pthread_cond_broadcast(&logger_thread_cond);
    if(light_flag)
    {
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      light_flag = 0;
      if(mq_receive(light_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
      {
        printf("Error receiving msg to  light queue\n");
      }
      logging_function(getppid(),getpid(),syscall(SYS_gettid),logging_thread->log_file,"Timer event light handler",msg);
    }
    if(temperature_flag)
    {
      temperature_flag = 0;
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      if(mq_receive(temperature_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
      {
        printf("Error receiving msg to  temperature queue\n");
      }
      logging_function(getppid(),getpid(),syscall(SYS_gettid),logging_thread->log_file,"Timer event temperature handler",msg);
    }
  }
  mq_close(temperature_discriptor);
  mq_close(light_discriptor);
}

void logging_function(int parent_id, int pthread_id, int thread_id, char *file_name,char* ip_str,msg_struct *msg)
{
  pthread_mutex_lock(&logger_mutex); 
  FILE *file_ptr = fopen(file_name,"a");
  struct timeval curr_time;
  gettimeofday(&curr_time, NULL);
  fprintf(file_ptr,"\n[Timestamp: %ld seconds]\n",(curr_time.tv_sec));
  if(msg != NULL)
  {
    if(strcmp(msg->level,"DATA") == 0)
      fprintf(file_ptr,"Thread:%s\nLog level:%s\nsensor_value:%.2f%c\nstate:%s\n",msg->thread_name,msg->level,msg->sensor_value,msg->unit,msg->state);
    else
      fprintf(file_ptr,"Thread:%s\nLog level:%s\n",msg->thread_name,msg->level,msg->sensor_value,msg->unit);
  }
  fprintf(file_ptr,"%s\n",ip_str);
  fflush(file_ptr);
  fclose(file_ptr);
  pthread_mutex_unlock(&logger_mutex); 
}


