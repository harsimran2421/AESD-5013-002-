#include "user_led.h"

void led_control(int led_color, int status)
{
	FILE* file_ptr;
	if (status != OFF && status != ON)
	{
		printf("\nINCORRECT LED STATUS\n");
	}
  if (led_color > LOAD && led_color < BLUE)
	{
		printf("INCORRECT LED NUMBER\n");
		exit(-1);
	}
  if(led_color == BLUE)
  {
			file_ptr = fopen("/sys/devices/ocp.3/gpio-leds.8/leds/beaglebone:green:usr0/brightness", "w");
			fprintf(file_ptr,"%i",status);
  }
  else if(led_color == RED)
  {
			file_ptr = fopen("/sys/devices/ocp.3/gpio-leds.8/leds/beaglebone:green:usr1/brightness", "w");
			fprintf(file_ptr,"%i",status);
  }
  else if(led_color == GREEN)
  {
			file_ptr = fopen("/sys/devices/ocp.3/gpio-leds.8/leds/beaglebone:green:usr2/brightness", "w");
			fprintf(file_ptr,"%i",status);
  }
  else if(led_color == DATA)
  {
			file_ptr = fopen("/sys/class/gpio/gpio68/value", "w");
			fprintf(file_ptr,"%i",status);
  }
  else if(led_color == CLOCK)
  {
			file_ptr = fopen("/sys/class/gpio/gpio26/value", "w");
			fprintf(file_ptr,"%i",status);
  }
  else if(led_color == LOAD)
  {
			file_ptr = fopen("/sys/class/gpio/gpio44/value", "w");
			fprintf(file_ptr,"%i",status);
  }
	fclose(file_ptr);
}

