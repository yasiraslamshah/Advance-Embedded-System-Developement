/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : initial_test.c
* Creation Date : 31-03-2019
* Last Modified : Sun 31 March 2019 00:07:10 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: source file contatining function definition for initial_test function where the startup temperature and light sensor test are run
* Functions:
*           intial_test() to test all sensors before creating threads
* References:
_._._._._._._._._._._._._._._._._._._._._.*/

/*user defined header files*/
#include "logger.h"
#include "temperature.h"
#include "light.h"
#include "test.h"


int intial_test(char *filename)
{
  /*temperature sensor test*/
  int result = main_temp();
  if(result == EXIT_FAILURE)
  { 
      printf("\nTemperature Sensor disconnected!\n");
      /*fill up the message structure*/
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"temperature",strlen("temperature"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"ALERT",strlen("ALERT"));
      msg->unit = '\0';
      /*log message*/
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"Temperature sensor not functional",msg);
      return EXIT_FAILURE;
  }
  else if(result == EXIT_SUCCESS)
  {
      printf("\nTemperature Sensor Successfully connected\n");
      /*fill up the message structure*/
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"INFO",strlen("INFO"));
      msg->unit = '\0';
      /*log message*/
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"Temperature sensor working",msg);
  }
  /*light sensor test*/
  result = main_light();
  if(result == EXIT_FAILURE)
  { 
      printf("lightSensor disconnected!\n\n");
      /*fill up the message structure*/
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"ALERT",strlen("ALERT"));
      msg->unit = '\0';
      /*log message*/
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"light sensor not functional",msg);
      return EXIT_FAILURE;
  }
  else if(result == EXIT_SUCCESS)
  {
      printf("Light sensor successfully connected\n\n");
      /*fill up the message structure*/
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"INFO",strlen("INFO"));
      msg->unit = '\0';
      /*log message*/
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"Light sensor working",msg);
  }
  return EXIT_SUCCESS;
}

