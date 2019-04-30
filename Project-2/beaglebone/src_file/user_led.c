/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : user_led.c
* Creation Date :28-04-2019
* Last Modified : Sun 28 April 2019 20:10:11 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file to control on board leds
* Functions:
*           led_control() to turn on board leds on or off based on the inputs
* References:
_._._._._._._._._._._._._._._._._._._._._.*/

/*user libraries*/
#include "user_led.h"

/* -------------------------------*/
/**
 * @Synopsis function to control the on board leds. Pin 53,54 and 55 are used. 
 *
 * @Param led_color to select the led number
 * @Param status to take input from user to turn led on or off
 */
/* ---------------------------------*/
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

