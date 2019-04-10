/*Unit Test to test various functions related to temperature conversion
 * Author: Harsimransingh Bindra & Yasir Aslam Shah
 * Advance Embedded System Design
 * Spring 2019
 * Definition: Test module to test various functionality for the unit conversion .Includes the following functions:
 * Functions:Test_I2C_init(int file),Test_Read_Temperature_Celsius(int file),Test_Read_Temperature_Kelvin(int file),
 * Test_Read_Temperature_Farenheit(int file),MY_ASSERT_NOT_EQUAL(int result)
 * */

//Standard Library header
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

//User Defined Header
#include "temp.h"
#include "i2c.h"
#include "test_conversion.h"

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
int Test_I2C_init_Conversion(int file)
{
	printf("\nTest I2C Init:");
	int result = I2C_init(&file,1);
	MY_ASSERT_NOT_EQUAL(result);
	int result1=MY_ASSERT_NOT_EQUAL(result);
  if(result1 == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
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
int Test_Read_Temperature_Celsius(int file)
{
	printf("\nTest Temperature in Celsius:");
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
int Test_Read_Temperature_Kelvin(int file)
{
	printf("\nTest Temperature in Kelvin:");
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
int Test_Read_Temperature_Farenheit(int file)
{
	printf("\nTest Temperature in Farenheit:");
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
 * @Synopsis  :test function to assert if the expected value is not equal to
 * the return value of the test function, and returns PASS if not equal to the 
 * EXIT_FAILURE or Fail if equal
 *
 * @Param result: the result contains the return value of the tested function 
 */
/* ----------------------------------------------------------------------------*/
int MY_ASSERT_NOT_EQUAL(int result)
{
	if(result != status)
	{
		printf("Pass!\n");
	}
	else
  {
    printf("Fail!");
  }
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :test function to test the unit testing functions
 *
 * @Param result: the result contains the return value of the tested function 
 */
/* ----------------------------------------------------------------------------*/
int main_conversion(void)
{
	int file;

	Test_I2C_init_Conversion(file);
	
  Test_Read_Temperature_Celsius(file);
  Test_Read_Temperature_Kelvin(file);	
  Test_Read_Temperature_Farenheit(file);
	return 0;  
}
