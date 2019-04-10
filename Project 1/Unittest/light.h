/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : light.h
* Creation Date : 20-03-2019
* Last Modified : Wed 31 March 2019 00:07:26 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file to contatainging function declaration for light sensors
* 
* Functions:
*           - Light_main() function is first initializes the I2C bus then calls the Read_Data function. 
*           - Read_Data() it calculates the lux value based on channel 1 and channel 2 values depending on the datasheet
*           - Read_Light_Sensor() it calculates the lux value based on channel 1 and channel 2 values depending on the datasheet 
*           - Turn_on_Light_sensor() turns the sensor on
*           - Check_PowerUp() to test if it successfully powers up the sensor
*           - light_function() light pthread spawned calls this function
*           - light_handler() light timer handler which logs the light value periodically by addding new light value to the temp queue.
* 
* References:
*           -https://www.broadcom.com/products/optical-sensors/ambient-light-photo-sensors/apds-9301
*           -https://learn.sparkfun.com/tutorials/apds-9301-sensor-hookup-guide/all         
_._._._._._._._._._._._._._._._._._._._._.*/

#ifndef LIGHT_H
#define LIGH_H
/*standard libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

/*used defined libraries*/
#include "i2c.h"


//macros for Light Sensor APDS 9301
#define Power_Up 0x03
#define Power_Up_Return 0x33
#define Command_Control 0x80
#define Command_Reg 0x00
#define Timing_Reg 0x01
#define Data_Low0 0x0c
#define Data_High0 0x0D 
#define Data_Low1 0x0E
#define Data_High1 0x0F
#define Sensor_ID 0x0A
#define Set_Gain 0x12
#define Interrupt_Control_reg_Disable 0x06
#define Interrupt_Control_reg_Enable 0x06
#define Threshold_LL 0x02
#define Threshold_LH 0x03
#define Threshold_HL 0x04
#define Threshold_HH 0x05


//global variables
int file;
float Lux_Value;

/*functions*/

/* -------------------------------*/
/**
 * @Synopsis init function for light sensor to initialise and read
 *
 * @Param light_value to return the light value to the calling function
 *
 * @Returns exit status to indicate whether light value was read 
 * succcessfully from the sensor or an Error
 */
/* ---------------------------------*/
int Light_main();
/* -------------------------------*/
/**
 * @Synopsis check the current state of based on Lux values whether it is  
 * currently dark or light
 *
 * @Param file to provide path for i2c bus
 * @Param Lux input light value 
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int State(int file, int Lux);
/* -------------------------------*/
/**
 * @Synopsis read function to read LUX Low and High Values 
 *
 * @Param file  to provide path for i2c bus
 * @Param flag  to select data channels -CH0(16 bits) CH1(16 bits)
 *
 * @Returns the light lux value
 */
/* ---------------------------------*/
uint16_t Read_Data(int file, int flag,uint16_t *Final);

//function to read lux value
/* -------------------------------*/
/**
 * @Synopsis it calculates the lux value based on channel 1 and channel 2 
 * values depending on the datasheet
 *
 * @Param file  to provide path for i2c bus 
 * @Param light_value to return the light value to the calling function
 *
 * @Returns returns the lux value based on datasheet calculation
 */
/* ---------------------------------*/
int Read_Light_Sensor(int file);

//function to check sensor is working by power up

/* -------------------------------*/
/**
 * @Synopsis firstly is powers up the light sensor then sets the gain for the sensor. 
 * Next the intergration time is set to 402ms based on Set_Gain macro. We change the 
 * control register value of the light sensor in this function
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Turn_on_Light_sensor(int file);

//function to see if power up is working
/* -------------------------------*/
/**
 * @Synopsis to test whether the control register was changed successfully or not
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Check_PowerUp(int file);

/* -------------------------------*/
/**
 * @Synopsis Read_Sensor_Id() reads the sensor ID connected to the I2C bus
 *
 * @Param file to provide path for i2c bus
 * @Param data it returns the sensor ID read.
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Read_Sensor_ID(int file,uint8_t *data);
/* -------------------------------*/
/**
 * @Synopsis To check if the interrupt registers are written successfully or not
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Read_Interrupt(int file);

/* -------------------------------*/
/**
 * @Synopsis To write the interrupt register of the light sensor
 *
 * @Param file to provide path for i2c bus
 * @Param in_value the value to be written on the interrupt register
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Write_Interrupt(int file, uint8_t *in_value);  
/* -------------------------------*/
/**
 * @Synopsis to disable the interrupts on the sensor
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Disable_Interrupt_Control_Register(int file);

/* -------------------------------*/
/**
 * @Synopsis to enable the interrupts for the light sensor
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Enable_Interrupt_Control_Register(int file);
#endif
