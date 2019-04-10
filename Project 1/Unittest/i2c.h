/*I2C File
 * Author: Harsimransingh Bindra & Yasir Aslam Shah
 * Project 1
 * Advance Embedded System Design
 * Spring 2019
 *
 * Description: This file contains functions to write/read to and from the slave.In this 
 * project TMP102 (Temperature) and ADPS 9301(Light) sensors are used as the slave and the 
 * Beagle Bone Black acts as the master.The data is read or written to the slave as a bye 
 * or bytes or a word depending upon the slave register configuration.
 * The following file contains the following functions as: I2C_init(),write_control(),
 * write_timing(),Temp_Conversion(),I2C_Read_Byte_Data(),I2C_Read_Word(),I2C_Write_Byte,
 * I2C_Write_Word,I2C_Write_Bytes.
 *
 * */
#ifndef I2C_H
#define I2C_H

//header files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

//macros
#define Temper 1
#define Light 2

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  initializing function for I2C to define I2C path and connect and allocate the I2C Bus to teh slave
 *
 * @Param file - File provides the path to the I2C bus
 * @Param sensorv-Sensor parameter define if I2C is to be provided to the Light sensor or Temp sensor, 1:Temperature Sensor, 2:Light Sensor
 *
 * @Returns: Exit status based on success or failure   
 */
/* ----------------------------------------------------------------------------*/
int I2C_init(int *file, uint8_t sensor);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Function to write control registers to the APDS Sensor,Like Powering ON the sensor
 *
 * @Param file: File provides the Path to the I2C bus
 * @Param data:Data provides the data to be written to the control register as , for powering up a data of 0x03 is written to the control register
 *
 * @Returns   :Exit Status based on success or failure
 */
/* ----------------------------------------------------------------------------*/
uint8_t write_control(int file, uint8_t data);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Function to write to the timing registers for the APDS Light sensor.A value of 8 bits is written ORed with Command_Control(0x80) to write to the timing registers 
 *
 * @Param file: File provides the Path to teh I2C bus
 * @Param data: Data provids the value to be written to the timing registers
 *
 * @Returns   :Exit status based on success or failure
 */
/* ----------------------------------------------------------------------------*/
uint8_t write_timing(int file, uint8_t data);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis:Function to convert the temperature sensor values to different Units. Keelvin, to farenheit to Ceslius conversion is provided using this function  
 *
 * @Param temp: The Raw Temperature value read from the 16 bit register in the Temperature sensor
 * @Param unit: Unit provides the flexibility to user to select the final temperature unit,1:Ceslius, 2: Kelvin 3: Farenheit
 *
 * @Returns   :Exit status based on success or failure
 */
/* ----------------------------------------------------------------------------*/
float Temp_Conversion(float temp, int unit);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :I2C read function to read a byte of data from the I2C slave 
 *
 * @Param file: File provides the path to the I2C Bus
 * @Param data: Data pointer to store the 8bit data read from the slave 
 *
 * @Returns   :returns Exit status based on success or failure
 */
/* ----------------------------------------------------------------------------*/
int I2C_Read_Byte_Data(int file , uint8_t *data);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Function to read 2 bytes from the I2C slave
 *
 * @Param file: file paramter to provide the path of I2C Bus
 * @Param data: Return exit statu
 *
 * @Returns :Return exit status based on success or exit 
 */
/* ----------------------------------------------------------------------------*/
int I2C_Read_Word(int file , uint8_t *data);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Function to write to the I2C slave a data byte
 *
 * @Param file: file to provide the path of I2C bus
 * @Param data: 8 bit data to be written to the I2C slave
 *
 * @Returns   :exit status based on the success or failure
 */
/* ----------------------------------------------------------------------------*/
int I2C_Write_Byte(int file , uint8_t data);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Function to write to the I2C slave a word data
 *
 * @Param file: fiel to provide the path of I2C bus
 * @Param data: 32 bit data to be written to the I2C slave
 *
 * @Returns : Exit status based on the success or failure   
 */
/* ----------------------------------------------------------------------------*/
int I2C_Write_Word(int file , uint32_t *data);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Function to write to the I2C slave a data of two bytes
 *
 * @Param file: file to provide the path of I2C bus
 * @Param data: 16 bit data to be written to the I2C slave
 *
 * @Returns   :Exit status based on success or failure
 */
/* ----------------------------------------------------------------------------*/
int I2C_Write_Bytes(int file , uint16_t *data);
#endif


