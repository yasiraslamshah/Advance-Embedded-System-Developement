/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : logger.h
* Creation Date : 19-03-2019
* Last Modified : Sun 31 March 2019 00:10:11 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file for logger function declaration and structure declarations
* 
* Functions:
*           logging_thread() linked to logger pthread task
*           logging_function() to log messages in a common log file
*           timer_setup() to set timer attributes for heartbeat
* References:
_._._._._._._._._._._._._._._._._._._._._.*/

#ifndef _LOGGER_H
#define _LOGGER_H

/*standard headers*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <errno.h>

/*system headers*/
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/stat.h>

/*POSIX thread*/
#include <pthread.h>  

/*IPC headers*/
#include <netinet/in.h> 
#include <mqueue.h>
#include <netdb.h>

/*I2C headers*/
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/*user libraries*/
#include "user_led.h"

#ifndef DEBUG
#define perror(...) 
#endif

pthread_mutex_t bus_lock, heartbeat_mutex, logger_mutex;
pthread_cond_t temp_thread_cond,light_thread_cond, logger_thread_cond;
int exit_flag;
int light_flag;
int temperature_flag;

/*structure to store thread attributes*/
typedef struct thread_content{

    pthread_t tid;         // the thread identifier
    char* log_file;

}thread_struct;

/*structure to store log messages*/
typedef struct message_struct
{
  char thread_name[20];
  char level[6];
  char state[6];
  float sensor_value;
  char unit;

}msg_struct;


/* -------------------------------*/
/**
 * @Synopsis linked to logger pthread task used for synchronized logging
 *
 * @Param arg to pass arguement to pthread 
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
void *logging_thread(void *arg);

/* -------------------------------*/
/**
 * @Synopsis to log recieved message to a common file
 *
 * @Param parent_id - get parent ID process
 * @Param pthread_id - ID of the pthread sending the message
 * @Param thread_id - thread ID of the logging message
 * @Param file_name - file where the message should be logged
 * @Param ip_str  - user message
 * @Param msg - msg structure containing data
 */
/* ---------------------------------*/
void logging_function(int parent_id, int pthread_id, int thread_id, char *file_name,char* ip_str,msg_struct *msg);


/* -------------------------------*/
/**
 * @Synopsis to setup the heartbeat timer attrbutes
 *
 * @Param second_value - seconds interval value
 * @Param nanosec_value - nanosec interval value
 *
 * @Returns the timerspec struct with the required attributes set
 */
/* ---------------------------------*/
struct timespec timer_setup(uint32_t second_value, uint32_t nanosec_value);

#endif
