/*Name:i2c.h file*/
/*Author:*/
/* Project 1*/
/*Description:This header file contains includes,
and function defnition for i2c for project1*/

#ifndef I2C_H
#define I2C_H

#include "logger.h"

//macros
#define Temper 1
#define Light 2


//function defnition 
//init function to define i2c path and
//allocate the i2c bus 
/* -------------------------------*/
/**
 * @Synopsis  
 *
 * @Param file
 * @Param sensor
 *
 * @Returns   
 */
/* ---------------------------------*/
int I2C_init(int *file, uint8_t sensor);

//function to write control registers for 
//APDS Sensor
uint8_t write_control(int file, uint8_t data);

//function to write timing registers for 
//APDS Sensor
uint8_t write_timing(int file, uint8_t data);

//conversion function to deal with 
//UNITS for temperature from TMA 102
float Temp_Conversion(float temp, int unit);
//Read I2C data one single byte 
int I2C_Read_Byte_Data(int file , uint8_t *data);
//Read I2C data one single word 
int I2C_Read_Word(int file , uint8_t *data);
//Write I2C data one single byte
int I2C_Write_Byte(int file , uint8_t data);
//Write I2C data one single word
int I2C_Write_Word(int file , uint8_t *data);
//Write I2C data with 16bit data
int I2C_Write_Bytes(int file , uint16_t * data);
#endif
