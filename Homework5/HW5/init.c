/*
 * Author : Yasir Aslam Shah
 * FreeRTOS 8.2 Tiva Demo
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/rom_map.h"



// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "mythread.h"
#include "timers.h"
#include "init.h"
#include "init.h"
#include "mythread.h"
#include "main.h"

void initialize_uart(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 9600, 16000000);
}

void initialize_timer()
{
       TimerHandle_t myTimer = NULL;
       myTimer = xTimerCreate(
               "Timer1",
               pdMS_TO_TICKS(100),
               pdTRUE,
               (void *) pvTimerGetTimerID(myTimer),
               timer_handler
             );
      xTimerStart(myTimer, 0);
}

void initialize_gpio()
{
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
     GPIOPinConfigure(GPIO_PL1_I2C2SCL);
     GPIOPinConfigure(GPIO_PL0_I2C2SDA);
     GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
     GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
}


void initialize_i2c(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
    GPIOPinConfigure(GPIO_PL1_I2C2SCL);
    GPIOPinConfigure(GPIO_PL0_I2C2SDA);
    GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
    GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    I2CMasterInitExpClk(I2C2_BASE,SYSTEM_CLOCK , true);
}

void timer_handler(TimerHandle_t mytimer)
{
    static int count;
   xSemaphoreGive(semaphore_led);
    count++;
    if(count%10 == 0)
    {
        xSemaphoreGive( semaphore_temp);
    }
}

void alert_thread(void *pvParameters)
{
    log_struct log;
    for(;;)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
        strcpy(log.info, "ALERT:TEMP Triggered! More than 20 C!!!");
        log.data = 0;
        log.task_id = 1;
        log.tick_value = xTaskGetTickCount();
        if (xSemaphoreTake( sync_mutex, portMAX_DELAY) == pdTRUE);
        {
            xQueueSend(my_queue, (void * ) &log, (TickType_t )1);
            xSemaphoreGive(sync_mutex);
        }
    }
}
