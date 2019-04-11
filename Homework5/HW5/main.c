/*
 * Author : Yasir Aslam Shah
 * Code Example: Andy Kobyljanec
 * FreeRTOS 8.2 Tiva Demo
 *
 * main.c
 *
 * Andy Kobyljanec
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
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



// Main function
int main(void)
{
    semaphore_led = xSemaphoreCreateBinary();
    semaphore_temp = xSemaphoreCreateBinary();
    sync_mutex = xSemaphoreCreateMutex();
    ROM_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),SYSTEM_CLOCK);
    PinoutSet(false, false);
    my_queue = xQueueCreate( 10, sizeof(log_struct) );
    if( my_queue == NULL )
       {
           return 0;
       }
    /*system initialization*/
    initialize_uart();
    initialize_gpio();
    initialize_i2c();
    initialize_timer();

    if(xTaskCreate(led_thread, (const portCHAR *)"myled", 128, NULL, 1, NULL) != pdTRUE)
    {
        return 0;
    }
    if(xTaskCreate(temperature_thread, (const portCHAR *)"Mytemperature", 128, NULL, 1, NULL) != pdTRUE)
    {
        return 0;
    }
    if(xTaskCreate(logger_thread, (const portCHAR *)"My logging task", 128, NULL, 1, NULL) != pdTRUE)
    {
        return 0;
    }
    if(xTaskCreate(alert_thread, (const portCHAR *)"MyAlert", 128, NULL, 2,  &a_task) != pdTRUE)
    {
        return 0;
    }
    vTaskStartScheduler();
    return 0;
}


int my_temp_function(double *data)
{
    int32_t conversion = 0;
    uint8_t data_val[2];
    if(temp_read(data_val) != SUCCESSFULL)
    {
        return FAIL;
    }
    if (*(data_val + 0) & 0x01)
    {

        conversion = (*(data_val + 1) << 5) | (*(data_val + 0) >> 3);
        if (conversion > 0xFFF)
        {
            conversion |= 0xE000;
        }
    }
    else
    {
        conversion= (*(data_val + 1) << 4) | (*(data_val + 0) >> 4);
        if (conversion > 0x7FF)
        {
            conversion |= 0xF000;
        }
    }
        *data= (conversion * 0.0625);
    return SUCCESSFULL;
}

int temp_read(uint8_t *data)
{
    ROM_I2CMasterSlaveAddrSet(I2C2_BASE, 0x48, false);
    ROM_I2CMasterDataPut(I2C2_BASE, 0x00);
    ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    //waiting as master is busy
    while(!ROM_I2CMasterBusy(I2C2_BASE));
    //waiting as master is busy
    while(ROM_I2CMasterBusy(I2C2_BASE));
    uint32_t resp;
     resp = ROM_I2CMasterErr(I2C2_BASE);
     if(resp != I2C_MASTER_ERR_NONE)
     {
         return FAIL;
     }
    ROM_I2CMasterSlaveAddrSet(I2C2_BASE, 0x48, true);
    ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    //waiting as master is busy
    while(!ROM_I2CMasterBusy(I2C2_BASE));
    //waiting as master is busy
    while (ROM_I2CMasterBusy(I2C2_BASE));
    resp = ROM_I2CMasterErr(I2C2_BASE);
    if(resp != I2C_MASTER_ERR_NONE)
    {
        ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
        return FAIL;
    }
    else
    {
    *(data + 1) = ROM_I2CMasterDataGet(I2C2_BASE);
    ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    //waiting as master is busy
    while(!ROM_I2CMasterBusy(I2C2_BASE));
    //waiting as master is busy
    while (ROM_I2CMasterBusy(I2C2_BASE));
    resp = ROM_I2CMasterErr(I2C2_BASE);
    if(resp != I2C_MASTER_ERR_NONE)
    {

        ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
        return FAIL;
    }
    else
    {

        *(data + 0) = (uint8_t)ROM_I2CMasterDataGet(I2C2_BASE);
        ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    }
    return SUCCESSFULL;
    }
}

void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}


