/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : temperature.h
* Creation Date : 21-03-2019
* Last Modified : Wed 31 March 2019 00:02:56 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file to contatainging function declaration for temperature
* 
* Functions:
*           -temp_main() function is first initializes the I2C bus then calls the 
	     read_temperature function. 
*           -Read_Temperature() read function to read data from TMA102 sensor in C,K,F
*           -temperature_function() temperature pthread spawned calls this function which 
             initalises the temperature queue intializes the timer parameters and starts 
             a timer which logs temperature value periodically
*           -temperature_handler() Temperature timer handler which logs the temperature 
             periodically by addding new temperature value to the temp queue.
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
            -http://www.ti.com/lit/ds/symlink/tmp102.pdf
*           -https://www.sparkfun.com/products/13314  
*           
_._._._._._._._._._._._._._._._._._._._._.*/

//user defined library
#include "temp.h"
#include "i2c.h"

int val;
int temp_main(void)
{
  int unit=1;
  int result=I2C_init(&file,1);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  result = Read_Temperature(file,1);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = Read_Temperature(file,2);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Reading Failed!\n");
    return EXIT_FAILURE;
  } 
  result = Read_Temperature(file,3);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Reading Failed!\n");
    return EXIT_FAILURE;
  }
return EXIT_SUCCESS;

}


//read function to read data from TMA102 sensor in C,K,F
int Read_Temperature(int file,int unit)
{

  int result=I2C_Write_Byte(file,TMPSensor_Register);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  usleep(500);
  char buffer[1];
  result=I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  } 
	
  int temperature;
  temperature = ((buffer[0]) << 8) | (buffer[1]);
  temperature >>= 4;

  //correcting the signed bit
	if (temperature & (1 << 11))
  {
    temperature = temperature | Minus_Correction;
  }
  float  final_temperature= temperature * Celsius;
  if(unit == 1)
  {
    final_temperature= temperature * Celsius;
	  printf("Temp:%02fC  ", final_temperature);
  }
  else if(unit == 2)
  {
    float final_temperature_F = Temp_Conversion(final_temperature,unit);
    printf("Temp:%02fF  ", final_temperature_F);

  }
  else if(unit == 3)
  {
    float final_temperature_K = Temp_Conversion(final_temperature,unit);
    printf("Temp:%02fK  ", final_temperature_K);
 }
    usleep(10);
  return EXIT_SUCCESS;
}

int Write_Pointer_Reg(int file, uint8_t value)
{
	int result = I2C_Write_Byte(file,value);
  	if(result == EXIT_FAILURE)
  	{
    		perror("\nError: Failed to Write to Pointer!\n");
    		return EXIT_FAILURE;
  	}
}

int Read_TLow(int file)
{
  char buffer[1];	
  int result = I2C_Write_Byte(file,TMPSensor_TLow);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  int temperature=0;
  Write_Configuration(file);
  float temp = 0.0;

  if(val == 1)
  {
  	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 3;
	temp = temperature * Celsius;
  }
  else 
  {
	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 4; 
  	temp = temperature * Celsius ;
  }	
  printf("Read Value TLow is %f ",temp); 
 
  return EXIT_SUCCESS;
}

int Read_THigh(int file)
{
  int result = I2C_Write_Byte(file,TMPSensor_THigh);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  uint8_t buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  float temp= 0.0;
  int temperature = 0;
  Write_Configuration(file);
  if(val == 1)
  {
  	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 3; 
 	temp = temperature * Celsius ;
  }
  else 
  {
	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 4; 	
  	temp = temperature * Celsius ;
  }	
  printf("Read Value THigh is %f ",temp);
  return EXIT_SUCCESS;
}

int Read_Resolution(int file)
{
  uint16_t value = Resolution;
  int result=I2C_Write_Bytes(file,&value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read!\n");
    return EXIT_FAILURE;
  }
  unsigned char MSB =buffer[0];
  MSB = MSB & 0b01100000;
  if(MSB == 0x13)
  {
	printf("Resolution at 12 ");
  }
  else 
  {
 	printf("Resolutiion at 13 ");
  }
  return EXIT_SUCCESS;
}

int Read_Fault_Bits(int file)
{
  uint16_t value = Fault_Bits_4;	
 
  int result=I2C_Write_Bytes(file,&value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read!\n");
    return EXIT_FAILURE;
  }
  unsigned char MSB =buffer[0];
  unsigned char LSB= buffer[1];
  //MSB = MSB & 0b01111000;
  if(MSB ==0x13)
  {
	printf("Fault Bits 4 ");
  }
  else 
  {
 	printf("Fault Bits less than 4 ");
  }
  return EXIT_SUCCESS;
}
int Clear_ShutDown(int file)
{
  uint16_t value =ShutDown_Clear;	
  int result=Write_Pointer_Reg(file,value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read!\n");
    return EXIT_FAILURE;
  }
  unsigned char MSB =buffer[0];
  //MSB = MSB & 0b011000001;
  
  if(MSB == 0x60)
  {
	printf("ShutDown Mode Enable ");
  }
  else 
  {
 	printf("ShutDown Mode Disable ");
  }
  return EXIT_SUCCESS;
}
int Set_ShutDown(int file)
{
  uint16_t value =ShutDown_Set;	
  int result=Write_Pointer_Reg(file,value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read!\n");
    return EXIT_FAILURE;
  }
  unsigned char MSB =buffer[0];
  
  if(MSB == 0x13)
  {
	  printf("ShutDown Mode Enable ");
  }
  else 
  {
 	  printf("ShutDown Mode Disable ");
  }
  return EXIT_SUCCESS;
}
int Set_EM(int file)
{
  uint16_t value =EM_Set;	
  int result=Write_Pointer_Reg(file,value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read!\n");
    return EXIT_FAILURE;
  }
  unsigned char LSB =buffer[1];
  
  if(LSB ==0x10)
  {
	  printf("Extended Mode 13 Bit ");
  }
  else 
  {
 	  printf("Extended Mode 12 Bit ");
  }
  return EXIT_SUCCESS;
}
int Set_Conversion(int file)
{
  uint16_t value =TMPSensor_Conversion_8;	
  int result=Write_Pointer_Reg(file,value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write_Convert!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read_Convert!\n");
    return EXIT_FAILURE;
  }
  unsigned char LSB =buffer[1];
  
  if(LSB == 0x40)
  {
  	printf("1Hz Frequency set! ");
  }
  else if(LSB == 0xc0)
  {
  	printf("8Hz Frequency set! ");
  }
  else
  {
  	printf("4Hz Frequency Set by Default ");
  }
  return EXIT_SUCCESS;
}

int Write_Configuration(int file)
{
  uint16_t value =TMPSensor_Configuration;	
  int result=Write_Pointer_Reg(file,value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read!\n");
    return EXIT_FAILURE;
  }
  unsigned char LSB =buffer[1];
  LSB = LSB & Mask;
  if(LSB ==0x10)
  {
  	val =1;
  }
  else val =0;

  return EXIT_SUCCESS;
}
