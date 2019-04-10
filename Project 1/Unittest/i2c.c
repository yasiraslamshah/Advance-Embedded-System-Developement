/*I2C.c File
 * Author: Harsimransingh Bindra & Yasir Aslam Shah
 * Project 1
 * Advance Embedded System Design
 * Spring 2019
 * Description: This file contains functions to write/read to and from the slave.In this project TMP102 
 * (Temperature) and ADPS 9301(Light) sensors are used as the slave and the Beagle Bone Black acts as the 
 * master.The data is read or written to the slave as a bye or bytes or a word depending upon the slave 
 * register configuration.
 * The following file contains the following functions as: I2C_init(),write_control(),write_timing(),
 * Temp_Conversion(),I2C_Read_Byte_Data(),I2C_Read_Word(),I2C_Write_Byte,I2C_Write_Word,I2C_Write_Bytes.
 *
 * */

//Header files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

//User Defined Header Files 
#include "i2c.h"
#include "temp.h"
#include "light.h"


//init function to define the 2c path and allocate buses
int I2C_init(int *file,uint8_t sensor)
{
	int Slave_Address;	
	char *filename= "/dev/i2c-2";
	int temp_file;	
	temp_file=open(filename,O_RDWR);
	*file = temp_file;
  	if(temp_file < 0)
	{
		perror("Error :Cant Open File\n");
		return EXIT_FAILURE;  
	}
	
	if(sensor==Temper)
	{
		Slave_Address  = 0x48;
  }
	else if(sensor==Light)
	{	
		Slave_Address = 0x39;
	}
	if(ioctl(temp_file,I2C_SLAVE,Slave_Address)<0)
	{
		printf("Error:IOCTL Failed!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//write control function to write to the control registers
uint8_t write_control(int file, uint8_t data)
{
  //writing to the command register
  uint8_t value = Command_Control | Command_Reg;
  int result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  } 
  //writing to the control register
  result = I2C_Write_Byte(file,data);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

//write timing function to write to the timing registers
uint8_t write_timing(int file, uint8_t data)
{
  uint8_t value = Command_Control | Timing_Reg;
  int result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  } 
  result = I2C_Write_Byte(file,data);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

//temperature conversion function to change fro Celsius
//Klevina and Farenheit Units as per User Demand
float Temp_Conversion(float temp, int unit)
{
  if(temp == 0)
  {
    return EXIT_FAILURE;
  }
  float temperature;
  if(unit == 2)
  {
    temperature = (temp* FAREN) + HEIT;
  }
  else if(unit ==3 )
  {
    temperature = temp + KELVIN;
  }
  return temperature;

}
//Read I2C data one single byte 
int I2C_Read_Byte_Data(int file , uint8_t *data)
{

	if(read(file,data,1) != 1)
	{
		//perror("Error :I2C BYTE Data Reading Failed!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
//Read I2C data one single word 
int I2C_Read_Word(int file , uint8_t *data)
{
	if(read(file,data,2)!=2)
	{
		//perror("Error :I2C WORD Data Reading Failed!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//Write I2C data one single byte
int I2C_Write_Byte(int file , uint8_t data)
{
	if(write(file,&data,sizeof(data))!=1)
	{
		//perror("Error :I2C BYTE Data Write Failed!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//Write I2C data one single word
int I2C_Write_Word(int file , uint32_t *data)
{
	if(write(file,data,3)!=3)
	{
		//perror("Error : :I2C WORD Data Write Failed!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//Write I2C data with 16bit data
int I2C_Write_Bytes(int file , uint16_t * data)
{
	if(write(file,data,2)!=2)
	{
		//perror("Error : I2C BYTES Write Failed! \n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
