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

#include "light.h"


int file;
mqd_t ser_discriptor;


void *light_function(void *arg)
{
  struct mq_attr attribute;
  attribute.mq_maxmsg = 10;
  attribute.mq_msgsize = sizeof(msg_struct);
  attribute.mq_flags = 0;
  attribute.mq_curmsgs = 0;

  /*create a queue for transferring light value to logger task*/
  ser_discriptor = mq_open ("/light_queue", O_RDWR | O_CREAT, 0666, &attribute);
  if (ser_discriptor < 0)
  {
    printf("light message queue error\n");
  }
  else
  {
    printf("light message queue created successfully\n");
  }

  timer_t timer_id;
  thread_struct *light_thread = (thread_struct *)arg;
  struct sigevent sev;
  memset(&sev,0,sizeof(struct sigevent));
  struct itimerspec trigger;
  memset(&trigger,0,sizeof(struct itimerspec));

/*set timer event characteristics*/
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = &light_handler;
  sev.sigev_value.sival_ptr = light_thread->log_file;
  timer_create(CLOCK_REALTIME, &sev, &timer_id);

/*set timer capabilities*/
  trigger.it_value.tv_sec=2;
  trigger.it_value.tv_nsec=0;
  trigger.it_interval.tv_sec=2;
  trigger.it_interval.tv_nsec=0;
  /*enable timer*/
  timer_settime(timer_id,0, &trigger, NULL);
  /*wait for the exit flag*/
  while(exit_flag != 1);
  timer_delete(timer_id);
  /*close the queue before exit*/
  mq_close(ser_discriptor);
}

void light_handler(union sigval sv)
{
  /*For heartbeat implementation*/
  pthread_cond_broadcast(&light_thread_cond);
  static int i;
  char *file_name = sv.sival_ptr;
  FILE *file_ptr;
  float light_value = 0;
  static int error_status;
  /*read temperature periodically*/
  int result = Light_main(&light_value);
  if(result == EXIT_FAILURE)
  {
    error_status++;
    /*condition when sensor is plugged out*/
    if(error_status == 1)
    {
      int light_led_status = 1;
      /*turn an led on to indicate sensor failure*/
      led_control(GREEN,light_led_status);
      printf("\nlight Sensor Disconnected\n");
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"light",strlen("light"));
      memset(msg->level,'\0',sizeof(msg->level));
      msg->unit = '\0';
      /*send alert message to the logger task*/
      if(mq_send(ser_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
      {
        printf("Error sending to light queue\n");
        mq_close(ser_discriptor);
      }
      else
      {
        light_flag = 1;
      }
    }
  }
  /*condition is sensor is read successfully*/
  else if(result == EXIT_SUCCESS)
  {
    if(error_status != 0)
    {
      int light_led_status = 0;
      led_control(GREEN,light_led_status);
    }
    error_status = 0;
    msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
    memset(msg->thread_name,'\0',sizeof(msg->thread_name));
    memcpy(msg->thread_name,"light",strlen("light"));
    msg->sensor_value = light_value;
    memset(msg->level,'\0',sizeof(msg->level));
    memcpy(msg->level,"DATA",strlen("DATA"));
    msg->unit = 'L';
    memset(msg->state,'\0',sizeof(msg->state));
    light_state(light_value,msg); 
    printf("Light value is %0.2f%c\t\tState:%s\n\n",msg->sensor_value,msg->unit,msg->state);
    /*send light sensor value to light queue*/
    if(mq_send(ser_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
    {
      printf("Error sending to light queue\n");
      mq_close(ser_discriptor);
    }
    else
    {
      light_flag = 1;
    }
  }
  /*print error if sensor is disconnected*/
  if((error_status > 0))
  {
    printf("\nLight Sensor disconnected\n");
  }
}

int Light_main(float *light_value)
{
  /*lock mutex for synchronised I2C access*/
  pthread_mutex_lock(&bus_lock); 
  /*intialize the I2C bus*/
  int result = I2C_init(&file,2);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor Initialization Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  result = Turn_on_Light_sensor(file);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: SensorII Reading Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  
  result = Check_PowerUp(file);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: SensorII Reading Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  uint8_t SensorID;
  result = Read_Sensor_ID(file,&SensorID);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result =Read_Light_Sensor(file, light_value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: SensorII Reading Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  /*return the I2C bus mutex*/
  pthread_mutex_unlock(&bus_lock); 
  return EXIT_SUCCESS;

}

uint16_t Read_Data(int file, int flag)
{
  uint8_t LSB,MSB;
  uint16_t Final;
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
    perror("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&LSB);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: SensorII Reading Failed!\n");
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
    perror("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&MSB);
  if(result == EXIT_FAILURE)
  {
 //   printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  Final = (MSB<<8)|(LSB);
  //printf("\nValue is %d\n",Final);
  return Final;
}

//function to read lux values
int Read_Light_Sensor(int file, float *light_value)
{ float Lux_Value = 0;
  float data0 =(float)Read_Data(file,1);
  float data1 =(float)Read_Data(file,2);  
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
  *light_value = Lux_Value;
return EXIT_SUCCESS;
}

//function to check sensor is working by power up
int Turn_on_Light_sensor(int file)
{
  int result = write_control(file,Power_Up);
  if(result == EXIT_FAILURE)
  {
//    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  result = write_timing(file,0x12);
  if(result == EXIT_FAILURE)
  {
//    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
}

//function to see if power up is working
int Check_PowerUp(int file)
{
  int result = I2C_Write_Byte(file,Command_Control);
  if(result == EXIT_FAILURE)
  {
//    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  uint8_t buf[1];
  result = I2C_Read_Word(file,buf);
  if(result == EXIT_FAILURE)
  {
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
    perror("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,data);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor_ID Write Failed!\n");
    return EXIT_FAILURE;
  }
  if(*data == 0x50)
  {
    perror("\nSensorID Read Successfull!\n");
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
    perror("\nError: Sensor_ID 2Write Failed!\n");
    return EXIT_FAILURE;
  }
  //perror("\nValue in read%d", value); 
  value = Command_Control | Threshold_LH;
  result = I2C_Read_Byte_Data(file,&value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor_ID 2Write Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nValue in read%d", value); 


  value = Command_Control | Threshold_HL;
  result = I2C_Read_Byte_Data(file,&value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor_ID 2Write Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nValue in read%d", value); 

  
  value = Command_Control | Threshold_HH;
  result = I2C_Read_Byte_Data(file,&value);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Sensor_ID 2Write Failed!\n");
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
  //  printf("\nError: Sensor_ID Write Failed!\n");
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
    //printf("\nError: Interrupt disable Failed!\n");
    return EXIT_FAILURE;
  }
  value =0x23;
  result=I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Interrupt Enable Failed!\n");
    return EXIT_FAILURE;
  }

  int8_t data;
  result = I2C_Read_Byte_Data(file,&data);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Interrupt Disable Read Failed!\n");
    return EXIT_FAILURE;
  }
  //printf("\nRead Inter_Dis: %d %x\n",data,data);
  return EXIT_SUCCESS;
}

void light_state(float LUX, msg_struct *msg)
{
	if(LUX > LIGHT_THRESHOLD)
	{
    memcpy(msg->state,"light\0",strlen("light\0"));
	}
	else
  {
    memcpy(msg->state,"dark\0",strlen("dark\0"));
  }	
}

int Enable_Interrupt_Control_Register(int file)
{
  uint8_t value = Command_Control | Interrupt_Control_reg_Enable;
  int result = I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Interrupt Enable Failed!\n");
    return EXIT_FAILURE;
  }
  value =0x23;
  result=I2C_Write_Byte(file,value);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Interrupt Enable Failed!\n");
    return EXIT_FAILURE;
  }

  uint8_t data=0;
  result = I2C_Read_Byte_Data(file,&data);
  if(result == EXIT_FAILURE)
  {
    //printf("\nError: Interrupt Disable Failed!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}


