/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : light.c
* Creation Date : 20-03-2019
* Last Modified : Wed 31 March 2019 00:09:00 PM MDT
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
*           - light_test() function to test the light sensor during initial startup
* References:
*           -https://www.broadcom.com/products/optical-sensors/ambient-light-photo-sensors/apds-9301
*           -https://learn.sparkfun.com/tutorials/apds-9301-sensor-hookup-guide/all         
_._._._._._._._._._._._._._._._._._._._._.*/
//include headers
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <math.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c-dev.h>
#include<unistd.h>

//user defined header files
#include "i2c.h"
#include "light.h"

//global Variables
int file;
float Lux_Value;

int Light_main()
{
  //2 is for Sensor: Light
  int result = I2C_init(&file,2);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  result = Turn_on_Light_sensor(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  
  result = Check_PowerUp(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  uint8_t SensorID;
  //usleep(500);
  result = Read_Sensor_ID(file,&SensorID);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nSensorID = %d\n",SensorID);

  result =Read_Light_Sensor(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  printf("\nLux is %f\n",Lux_Value);
 
  result =State(file,Lux_Value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Status Failed!\n");
    return EXIT_FAILURE;
  }
  Enable_Interrupt_Control_Register(file);
  uint8_t in_value = 5;
  
  Write_Interrupt(file,&in_value);
  usleep(10000);
  Read_Interrupt(file);
  Disable_Interrupt_Control_Register(file);
  

  return EXIT_SUCCESS;
}

int State(int file,int LUX)
{
	if(LUX < 0)
	{
		return EXIT_FAILURE;
	}
	if(LUX >30)
	{
		printf("\nStatus: Light");
	}
	else printf("\nStatus: Dark");
	return EXIT_SUCCESS;

}
uint16_t Read_Data(int file, int flag,uint16_t *Final)
{
  uint8_t LSB,MSB;
  uint8_t address;
  if(flag == 1)
  {
    address =Command_Control | Data_Low0;
  }
  else if(flag ==2)
  {
    address =Command_Control | Data_Low1;
  }
    int result = I2C_Write_Byte(file,address);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&LSB);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
   if(flag == 1)
  {
    address =Command_Control | Data_High0;
  }
  else if(flag ==2)
  {
    address =Command_Control | Data_High1;
  }
  result = I2C_Write_Byte(file,address);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&MSB);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  *Final = (MSB<<8)|(LSB);
  //printf("\nValue is %d\n",Final);
  return EXIT_SUCCESS;
}

//function to read lux values
int Read_Light_Sensor(int file)
{
  uint16_t temp;
  int result=Read_Data(file,1,&temp);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  float data0 =(float)temp;
  result =Read_Data(file,2,&temp);  
	if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  float data1=(float)temp;
  if(data0 == 0)
	{
		perror("Error:Process Failed!");
		return EXIT_FAILURE;
	}
	
	float ADC= data1/data0;
  float power = pow(ADC,1.4);
	if(0 < ADC < 0.50)
	{
		Lux_Value = (0.0304 * data0) - (0.062 * data0 * power);
	}
	else if(0.50 < ADC < 0.61)
	{
		Lux_Value = (0.0224 * data0) - (0.031 * data1);
	}
	else if(0.61 < ADC < 0.80)
	{
		Lux_Value = (0.0128 * data0) - (0.0153 * data1);
	}
	else if(0.80 < ADC < 1.30)
	{
		Lux_Value = (0.00146 * data0) - (0.00112 * data1);
	}
	else
	{
		Lux_Value = 0;
	}
return EXIT_SUCCESS;

}

//function to check sensor is working by power up
int Turn_on_Light_sensor(int file)
{
  int result = write_control(file,Power_Up);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  //Setting the Gain for the sensor
  //Integration time set to 402ms 
  result = write_timing(file,Set_Gain);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

//function to see if power up is working
int Check_PowerUp(int file)
{
  int result = I2C_Write_Byte(file,Command_Control);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  uint8_t buf[1];
  result = I2C_Read_Word(file,buf);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  int value = buf[0];
  //printf("\nValue is %d %x\n",value,value);

  if(value == Power_Up_Return)
  {
    //printf("\nSensor Checkup Successfully!\n");
    return EXIT_SUCCESS;
  }
  else 
  {
    //printf("\nFailed!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Read_Sensor_ID(int file,uint8_t *data)
{
  uint8_t value = Command_Control | Sensor_ID;
  int result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,data);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  if(*data == 0x50)
  {
    //printf("\nSensorID Read Successfull!\n");
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
}

int Read_Interrupt(int file)
{
  uint8_t value = Command_Control | Threshold_LL;
  int data[3];
  uint8_t *dat;
  
  int result = I2C_Read_Byte_Data(file,&value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID 2Write Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nValue in read%d", value); 
  
  value = Command_Control | Threshold_LH;
  result = I2C_Read_Byte_Data(file,&value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID 2Write Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nValue in read%d", value); 


  value = Command_Control | Threshold_HL;
  result = I2C_Read_Byte_Data(file,&value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID 2Write Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nValue in read%d", value); 

  
  value = Command_Control | Threshold_HH;
  result = I2C_Read_Byte_Data(file,&value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID 2Write Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nValue in read%d", value); 
  return EXIT_SUCCESS;
}

int Write_Interrupt(int file, uint8_t *in_value)
{
  uint8_t value = Command_Control | Threshold_LL;
  int data[3];
  
  int result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  data[0]=0x01;	
  result = I2C_Write_Byte(file,data[0]);
  

  value = Command_Control | Threshold_LH;
  result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  data[1]=0x02;	
  result = I2C_Write_Byte(file,data[1]);
  

  value = Command_Control | Threshold_HL;
  result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  data[2]=0x03;	
  result = I2C_Write_Byte(file,data[2]);
 
  value = Command_Control | Threshold_HH;
  result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  data[3]=0x04;	
  result = I2C_Write_Byte(file,data[3]);  
  //printf("\nData write is %d %d %d %d \n",data[0],data[1],data[2],data[3]);
  return EXIT_SUCCESS;
}

int Disable_Interrupt_Control_Register(int file)
{
  uint8_t value = Command_Control | Interrupt_Control_reg_Disable;
  int result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Interrupt disable Failed!\n");
    return EXIT_FAILURE;
  }
  value =0x23;
  result=I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Interrupt Enable Failed!\n");
    return EXIT_FAILURE;
  }

  int8_t data;
  result = I2C_Read_Byte_Data(file,&data);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Interrupt Disable Read Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nRead Inter_Dis: %d %x\n",data,data);
  return EXIT_SUCCESS;
}


int Enable_Interrupt_Control_Register(int file)
{
  uint8_t value = Command_Control | Interrupt_Control_reg_Enable;
  int result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Interrupt Enable Failed!\n");
    return EXIT_FAILURE;
  }
  value =0x23;
  result=I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Interrupt Enable Failed!\n");
    return EXIT_FAILURE;
  }

  uint8_t data=0;
  result = I2C_Read_Byte_Data(file,&data);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Interrupt Disable Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nRead Inter_En: %d %x\n",data,data);
  return EXIT_SUCCESS;

}


