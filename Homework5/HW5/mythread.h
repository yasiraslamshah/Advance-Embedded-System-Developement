/*
 * Author : Yasir Aslam Shah
 * FreeRTOS 8.2 Tiva Demo
 */

#ifndef MYTHREAD_H_
#define MYTHREAD_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*driver libraries*/
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"

/*free rtos libraries*/
#include "FreeRTOS.h"
#include "main.h"
#include "queue.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "task.h"
/*Board defined libraries*/

#include "timers.h"
/*user defined libraries*/

#include "init.h"

xSemaphoreHandle sync_mutex;
SemaphoreHandle_t semaphore_led;
SemaphoreHandle_t semaphore_temp;
TaskHandle_t a_task ;
QueueHandle_t my_queue;

typedef struct{
    int task_id;
    TickType_t tick_value;
    char info[20];
    double data;
}log_struct;



void led_thread(void *pvParameters);
void logger_thread(void *pvParameters);
void temperature_thread(void *pvParameters);
void timerTask(TimerHandle_t mytimer);
void alert_thread(void *pvParameters);
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName);
#endif /* MYTHREAD_H_ */
