/*Unit Test to test various functions related to Temperature
 * Author: Harsimransingh Bindra & Yasir Aslam Shah
 * Advance Embedded System Design
 * Spring 2019
 * Definition: Test module to test various functionality for teh temperarture sensor.Includes the following functions:
 * Functions: Test_I2C_init(),Test_Read_Sensor_Celsius(),Test_Read_Sensor_Kelvin(),Test_Read_Sensor_Farenheit(),Test_Read_TLow(),
Test_Read_THigh(int file),Test_Read_Resolution(),Test_Read_Fault_Bits(),Test_Clear_ShutDown(),Test_Set_ShutDown(),
Test_Set_EM(),Test_Set_Conversion(),Test_Write_Configuration(),MY_ASSERT_NOT_EQUAL()
 * */

//Standard Header files
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

//User Defined Header files
#include "temp.h"
#include "test_temp.h"
#include "test_conversion.h"

//Macros
#define status EXIT_FAILURE


//function defnition:
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test function to test the I2C path and bus allocation to the slave(1)
 *
 * @Param file:File is used to allocate the I2C bus  and path to the slave
 *
 * @Returns   :returns exit status upon the success or failure of function
 */
/* ----------------------------------------------------------------------------*/
int Test_I2C_init_Temp(int file)
{
	printf("\nTest I2C Init:");
	int result = I2C_init(&file,1);
	int result1=MY_ASSERT_NOT_EQUAL(result);

}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test functiion to test the temperature sensor readaing in Celsius
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns the exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Sensor_Celsius(int file)
{
	printf("\nTest Read Celsius:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,1);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test functiion to test the temperature sensor readaing in Kelvin
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns the exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Sensor_Kelvin(int file)
{
	printf("\nTest Read Kelvin:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,2);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test functiion to test the temperature sensor readaing in Farenheit
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns the exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Sensor_Farenheit(int file)
{
	printf("\nTest Read Farenheit:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,3);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to read the Tlow value from the TMP102 Sensor,TLOW 
 * stands for thethreshold low value for the alert to occur
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_TLow(int file)
{
	printf("\nTest Read Sensor TLow:");
	I2C_init(&file,1);
	int result = Read_TLow(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to read the THigh value from the TMP102 Sensor,
 * THigh stands for thethreshold low value for the alert to occur
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_THigh(int file)
{
	printf("\nTest Read Sensor THigh:");
	I2C_init(&file,1);
	int result = Read_THigh(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to configure the sensor resolution  value from the 
 * TMP102 Sensor,the sensor has a 12 bit and a 13 bit resolution
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Resolution(int file)
{
	printf("\nTest Read Resolution:");
	I2C_init(&file,1);
	int result = Read_Resolution(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to configure the fault bits of sensor TMP102,a consecutive 
 * fault options of 1,2,4 and 6 are avaialble for the system to trigger the alert
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Read_Fault_Bits(int file)
{
	printf("\nTest Read Fault Bits:");
	I2C_init(&file,1);
	int result = Read_Fault_Bits(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to configure the shutdown feature for the temperature
 * sensor, setting the shutdown bit enables the shutdown mode
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Clear_ShutDown(int file)
{
	printf("\nTest ShutDown Disable:");
	I2C_init(&file,1);
	int result = Clear_ShutDown(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to configure the shutdown feature for the temperature
 * sensor, clearing the shutdown bit disables the shutdown mode
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Set_ShutDown(int file)
{
	printf("\nTest Read Set ShutDown:");
	I2C_init(&file,1);
	int result = Set_ShutDown(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to configure the Extended Mode, extended mode allows 13 bit 
 * temperature reading allowing to read temperature above 128C
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Set_EM(int file)
{
	printf("\nTest Read Set Extended Mode:");
	I2C_init(&file,1);
	int result = Set_EM(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to configure the conversion rate for the sensor, 
 * conversion rate of 1Hz,4Hz and 8Hz are avaialable with 4Hz as default
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Set_Conversion(int file)
{
	printf("\nTest Read Set Conversion:");
	I2C_init(&file,1);
	int result = Set_Conversion(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to configure the write configuration register to 
 * enable and disable various features in TMP 102
 *
 * @Param file: File is used to allocate the path and bus to the I2C slave
 *
 * @Returns   :returns exit status upon success or failure
 */
/* ----------------------------------------------------------------------------*/
int Test_Write_Configuration(int file)
{
	printf("\nTest Write Configuration:");
	I2C_init(&file,1);
	int result = Write_Configuration(file);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to assert if the expected value is not equal to
 * the return value of the test function, and returns PASS if not equal to the 
 * EXIT_FAILURE or Fail if equal
 *
 * @Param result: the result contains the return value of the tested function 
 */
/* ----------------------------------------------------------------------------*/
/*int MY_ASSERT_NOT_EQUAL(int result)
{
	if(result != status)
	{
		printf("Pass!\n");
		return EXIT_SUCCESS;
	}
	else 
  {
    printf("Fail!");
    return EXIT_FAILURE;
  }
}*/
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to run all the test functions for temperature sensor
 *
 * @Param result: the result contains the return value of the tested function 
 */
/* ----------------------------------------------------------------------------*/
int main_temp(void)
{
	int file;

int res = Test_I2C_init_Temp(file);
  res = Test_Read_Sensor_Celsius(file);
  res = Test_Read_Sensor_Kelvin(file);	
  res = Test_Read_Sensor_Farenheit(file);

  res = Test_Write_Configuration(file);
  res = Test_Read_TLow(file);

  res = Test_Read_THigh(file);

  res = Test_Read_Resolution(file);
  res = Test_Read_Fault_Bits(file);
  res = Test_Clear_ShutDown(file);
  res = Test_Set_ShutDown(file);
  res = Test_Set_EM(file);

  res = Test_Set_Conversion(file);

  res = Test_Write_Configuration(file);
  
  return 0;
}
