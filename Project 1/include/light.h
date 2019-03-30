/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : light.h
* Creation Date : 20-03-2019
* Last Modified : Wed 31 March 2019 00:07:26 PM MDT
* Created By : Harsimransingh
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
* 
* References:
*           
_._._._._._._._._._._._._._._._._._._._._.*/

#ifndef LIGHT_H
#define LIGH_H

/*standard libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

/*used defined libraries*/
#include "i2c.h"
#include "logger.h"
#include "user_led.h"

/*macros for Light Sensor APDS 9301*/
#define Power_Up 0x03
#define Command_Control 0x80
#define Command_Reg 0x00
#define Timing_Reg 0x01
#define Data_Low0 0x0c
#define Data_High0 0x0D 
#define Data_Low1 0x0E
#define Data_High1 0x0F

/*global variables*/
int file;
float Lux_Value;

/*functions*/

/* -------------------------------*/
/**
 * @Synopsis init function for light sensor to initialise and read
 *
 * @Param light_value to return the light value to the calling function
 *
 * @Returns exit status to indicate whether light value was read succcessfully from the sensor or an Error
 */
/* ---------------------------------*/
int Light_main(float *light_value);

/* -------------------------------*/
/**
 * @Synopsis read function to read LUX Low and High Values 
 *
 * @Param file  to provide path for i2c bus
 * @Param flag  to select data channels -CH0(16 bits) CH1(16 bits)
 *
 * @Returns the light lux value
 */
/* ---------------------------------*/
uint16_t Read_Data(int file, int flag);

//function to read lux value
/* -------------------------------*/
/**
 * @Synopsis it calculates the lux value based on channel 1 and channel 2 values depending on the datasheet
 *
 * @Param file  to provide path for i2c bus 
 * @Param light_value to return the light value to the calling function
 *
 * @Returns returns the lux value based on datasheet calculation
 */
/* ---------------------------------*/
int Read_Light_Sensor(int file, float *light_value);

//function to check sensor is working by power up

/* -------------------------------*/
/**
 * @Synopsis firstly is powers up the light sensor then sets the gain for the sensor. Next the intergration time is set to 402ms based on Set_Gain macro. We change the control register value of the light sensor in this function
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Turn_on_Light_sensor(int file);

//function to see if power up is working
/* -------------------------------*/
/**
 * @Synopsis to test whether the control register was changed successfully or not
 *
 * @Param file to provide path for i2c bus
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int Check_PowerUp(int file);

/* -------------------------------*/
/**
 * @Synopsis light pthread spawned calls this function which initalises the light queue, intializes the timer parameters and starts a timer which logs light value periodically
 *
 * @Param arg to pass any arguements to this function
 *
 * @Returns void as linked to pthread
 */
/* ---------------------------------*/
void *light_function(void *arg);

/* -------------------------------*/
/**
 * @Synopsis light timer handler which logs the light value periodically by adding new light value to the light queue. 
 *
 * @Param sv
 */
/* ---------------------------------*/
void light_handler(union sigval sv);

#endif
