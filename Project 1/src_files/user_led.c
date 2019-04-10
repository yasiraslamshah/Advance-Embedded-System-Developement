#include "user_led.h"

void led_control(int led_color, int status)
{
	FILE* file_ptr;
	if (status != OFF && status != ON)
	{
		printf("\nINCORRECT LED STATUS\n");
	}
  if (led_color > GREEN && led_color < BLUE)
	{
		printf("INCORRECT LED NUMBER\n");
		exit(-1);
	}
  if(led_color == BLUE)
  {
			file_ptr = fopen("/sys/class/gpio/gpio53/value", "w");
			fprintf(file_ptr,"%i",status);
  }
  else if(led_color == RED)
  {
			file_ptr = fopen("/sys/class/gpio/gpio54/value", "w");
			fprintf(file_ptr,"%i",status);
  }
  else if(led_color == GREEN)
  {
			file_ptr = fopen("/sys/class/gpio/gpio55/value", "w");
			fprintf(file_ptr,"%i",status);
  }
	fclose(file_ptr);
}

