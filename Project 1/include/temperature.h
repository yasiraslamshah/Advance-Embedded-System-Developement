/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : temperature.h
* Creation Date : 21-03-2019
* Last Modified : Wed 31 March 2019 00:02:56 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file to contatainging function declaration for temperature
* 
* Functions:
*           -temp_main() function is first initializes the I2C bus then calls the read_temperature function. 
*           -Read_Temperature() read function to read data from TMA102 sensor in C,K,F
*           -temperature_function() temperature pthread spawned calls this function which initalises the temperature queue intializes the timer parameters and starts a timer which logs temperature value periodically
*           -temperature_handler() Temperature timer handler which logs the temperature periodically by addding new temperature value to the temp queue.
*           -temp_test() used for intial testing of the sensor
*           -temp_state() for knowing the current state
*           -Write_pointer_reg()
*           -Read_Tlow()
*           -Read_Thigh()
*           -Read_Resolution()
*           -Read_Fault_Bits()
*           -Set_ShutDown()
*           -Clear_ShutDown()
*           -Set_EM()
*           -Set_Conversion()
*           -Write_Configuration()
* References:
*           -http://www.ti.com/lit/ds/symlink/tmp102.pdf
*           -https://www.sparkfun.com/products/13314  
*           
_._._._._._._._._._._._._._._._._._._._._.*/




#ifndef TEMP_H
#define TEMP_H
/*Standard c libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

/*User defined libraries*/
#include "logger.h"
#include "i2c.h"

/*macros for temperature sensor*/
#define TMPSensor_Register 0x00
#define Minus_Correction 0xF800
#define Celsius 0.0625
#define KELVIN 273.5
#define FAREN 1.8
#define HEIT 32
#define TMPSensor_Conversion_8 0xC0
#define TMPSensor_Conversion_4 0x10
#define TMPSensor_Conversion_1 0x40
#define EM_Set 0x10
#define ShutDown_Set 0x6100
#define ShutDown_Clear 0x6000
#define Fault_Bits_4 0x7800
#define Fault_Bits_1 0x6000
#define Resolution 0x6000
#define TMPSensor_Configuration 0x01
#define TMPSensor_TLow 0x02
#define TMPSensor_THigh 0x03
#define Mask 0b00010000
#define KELVIN_THRESHOLD 298
#define FARHENIET_THRESHOLD 77
#define CELSIUS_THRESHOLD 25
/*  1 - celsius
 *  2 - Kelvin
 *  3 - Farheniet
 * */
#define UNIT 1
/*global variable*/
int file;

/* -------------------------------*/
/**
 * @Synopsis temp_main() function is first initializes the I2C bus then calls the read_temperature function.
 *
 * @Param temp_value  used to return the temperature value to the callin function
 * @Param unit  used to indicate which is required by the calling function(Celcius, Kelvin or Farheniet)
 *
 * @Returns Exit status whether the temperature calculation was a success or not
 */
/* ---------------------------------*/
int temp_main(float *temp_value,int unit);

/* -------------------------------*/
/**
 * @Synopsis  read function to read data from TMA102 sensor in C,K,F
 *
 * @Param file 
 * @Param unit used to indicate which is required by the calling function(Celcius, Kelvin or Farheniet)
 * @Param temp_value used to return the temperature value to the callin function
 *
 * @Returns Exit status whether the temperature calculation was a success or not
 */
/* ---------------------------------*/
int Read_Temperature(int file,int unit, float *temp_value);

/* -------------------------------*/
/**
 * @Synopsis temperature pthread spawned calls this function which initalises the temperature queue intializes the timer parameters and starts a timer which logs temperature value periodically
 *
 * @Param arg Nothing is passed to this function
 *
 * @Returns void as linked to phtread
 */
/* ---------------------------------*/
void *temperature_function(void *arg);


/* -------------------------------*/
/**
 * @Synopsis Temperature timer handler which logs the temperature periodically by addding new temperature value to the temp queue.
 *
 * @Param sv  used to pass the log file name.
 */
/* ---------------------------------*/
void temperature_handler(union sigval sv);

/* -------------------------------*/
/**
 * @Synopsis this is called during the startup to test whether the temperature sensor is functional or not
 *
 * @Returns returns exit status based on success or failure
 */
/* ---------------------------------*/
int temp_test();

/* -------------------------------*/
/**
 * @Synopsis this function checks to current state based on temperature threshold values
 *
 * @Param temp_value - provides current temperature value
 * @Param msg - msg structure where state needs to be stored
 * @Param unit - temperature unit
 */
/* ---------------------------------*/
void temp_state(float temp_value, msg_struct *msg, int unit);

/* -------------------------------*/
/**
 * @Synopsis used to write the pointer register of TMP 102 sensor
 *
 * @Param file  to provide path for i2c bus
 * @Param value provides value to be written in the register
 *
 * @Returns exit status based on success or failure 
 */
/* ---------------------------------*/
int Write_pointer_reg(int file, uint8_t value);

/* -------------------------------*/
/**
 * @Synopsis reads the lower threshold value of TMP 102 sensor
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Read_TLow(int file);

/* -------------------------------*/
/**
 * @Synopsis reads the highe threshold value of TMP 102 sensor
 *
 * @Param file  to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Read_THigh(int file);

/* -------------------------------*/
/**
 * @Synopsis reads the current resolution on which the TMP 102 sensor is set
 *
 * @Param file  to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Read_Resolution(int file);

/* -------------------------------*/
/**
 * @Synopsis reads which fault bits are set currrently.
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Read_Fault_Bits(int file);

/* -------------------------------*/
/**
 * @Synopsis it sets the shutdown bit to reduce current consumption
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Set_ShutDown(int file);

/* -------------------------------*/
/**
 * @Synopsis clears the shutdown bit to make the sensor completely functional
 *
 * @Param file  to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Clear_ShutDown(int file);

/* -------------------------------*/
/**
 * @Synopsis`it changes the resolution of the sensor from 12 bits to 13 bits if wider temperature range is required
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Set_EM(int file);

/* -------------------------------*/
/**
 * @Synopsis it changes the read frequency of sensor between 1,4 and 8 Hz
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Set_Conversion(int file);

/* -------------------------------*/
/**
 * @Synopsis it writes the configuration register to access different sensor modes
 *
 * @Param file  to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Write_Configuration(int file,int val);

#endif
