/*
 * Author : Yasir Aslam Shah
 * FreeRTOS 8.2 Tiva Demo
 */

#include <mythread.h>
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "init.h"
#include "mythread.h"

//temperature function
void temperature_thread(void *pvParameters)
{
    log_struct log;
    double data;
    while(1)
    {
        if (xSemaphoreTake(semaphore_temp,portMAX_DELAY) == pdTRUE)
        {
            my_temp_function(&data);
            if (data >= 20)
            {
                xTaskNotifyGive(a_task);

            }
            memset(log.info,'\0',sizeof(log.info));
            log.data = data;
            log.task_id = 3;
            strcpy(log.info, "Temperature Task");
            log.tick_value = xTaskGetTickCount();
            if (xSemaphoreTake( sync_mutex, ( TickType_t )20 ) == pdTRUE)
                ;
            {
                xQueueSend(my_queue, (void * ) &log, (TickType_t )1);
                xSemaphoreGive(sync_mutex);
            }
        }
    }
}
// flashing leds on the board
void led_thread(void *pvParameters)
{
    log_struct log;
    int32_t count = 0;
    while(1)
    {
        if (xSemaphoreTake(semaphore_led, portMAX_DELAY) == pdTRUE)
         {
             log.task_id = 2;
             log.data = count++;
             LEDWrite(0x01, 0x01);
             vTaskDelay(pdMS_TO_TICKS(2));
             LEDWrite(0x01, 0x00);
             memset(log.info,'\0',sizeof(log.info));
             strcpy(log.info, "Led Thread!");
             log.tick_value = xTaskGetTickCount();
             if (xSemaphoreTake( sync_mutex, ( TickType_t )20 ) == pdTRUE)

             {
                 xQueueSend(my_queue, (void * ) &log,
                            (TickType_t )1);
                 xSemaphoreGive(sync_mutex);
             }

             vTaskDelay(pdMS_TO_TICKS(2));
         }
    }
}

void logger_thread(void *pvParameters)
{
    log_struct log;
    while(1)
    {
        {
            if (xQueueReceive(my_queue, &log,(TickType_t ) 1) == pdTRUE)
            {
                if(log.task_id == 1)
                {
                    UARTprintf("\n\n\rAuthor: Yasir[Timestamp: %u milliseconds]\nALERT TASK:%s\n\r ",log.tick_value,log.info);
                }
                else if(log.task_id == 2)
                 {
                     UARTprintf("\n\n\rAuthor: Yasir[Timestamp:%u milliseconds]\n\nTrigger count = %u\n%s\n\r ",log.tick_value,(uint32_t) log.data, log.info);
                 }
                else if(log.task_id == 3)
                {
                    UARTprintf("\n\n\rAuthor: Yasir[Timestamp:%u milliseconds]\nData value = %uC\n%s\n\r ",log.tick_value,(uint32_t) log.data, log.info);
                }
             }
         }
    }
}
