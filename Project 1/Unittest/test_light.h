/*Unit Test to test various functions related to Light Sensor APDS 9301
 * Author: Harsimransingh Bindra & Yasir Aslam Shah
 * Advance Embedded System Design
 * Spring 2019
 * Definition: Test module to test various functionality for the Light sensor.Includes the following functions:
 * Functions:MY_ASSERT_NOT_EQUAL(),Test_I2C_init(),Test_Turn_on_light_sensor(),Test_Check_PowerUp(),
Test_Read_Sensor_ID(),Test_Light_Sensor(),Test_Write_Interrupt(),Test_Read_Interrupt(),int Test_Status()
 * */

#ifndef TEST_LIGHT_H
#define TEST_LIGHT_H


//Standard Header Files
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

//User Defined Header Files
#include "light.h"
//macros
#define status EXIT_FAILURE



//Function Defnition:
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to test the I2C path and bus allocation to the slave(2)
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_I2C_init_Light(int file);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to test the powering up of Sensor by writing to the 
 * control register
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Turn_on_light_sensor(int file);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to check if the powering up the sensor is successfull
 * or not 
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Check_PowerUp(int file);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to read the sensor id of Light APDS 9301 to ensure the
 * I2C is working or not
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Sensor_ID(int file,uint8_t SensorID);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to test the Light sensor and ensure if the values 
 * returned are correct and changes as per a change in ambient light around the
 * sensor
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Light_Sensor(int file);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to test the Light sensor by reading from the 16bit 
 * channels from the sensor
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Data(int file);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to configure the interrupt by writing the High High, 
 * High Low, Low High and Low Low to the Sensor.
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 * @Param SensorID: is a paramameter to select the Light Sensor as SensorID:2
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Write_Interrupt(int file, uint8_t SensorID);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to check if the values returned after writing the 
 * interrupt values are returned to be same after reading them back
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Interrupt(int file);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to check the state of the system to be DARK or LIGHT 
 * based on the LUX values
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_Status(int file);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to assert if the expected value is not equal to
 * the return value of the test function, and returns PASS if not equal to the 
 * EXIT_FAILURE or Fail if equal
 *
 * @Param result: the result contains the return value of the tested function 
 */
/* ----------------------------------------------------------------------------*/
//int MY_ASSERT_NOT_EQUAL(int result);
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to run all the test functions for the light sensor
 *
 * @Param result: the result contains the return value of the tested function 
 */
/* ----------------------------------------------------------------------------*/
int main_light(void);
#endif
