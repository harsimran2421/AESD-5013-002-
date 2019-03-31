/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : temperature.h
* Creation Date : 21-03-2019
* Last Modified : Wed 31 March 2019 00:02:56 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file to contatainging function declaration for temperature
* 
* Functions:
*           -temp_main() function is first initializes the I2C bus then calls the read_temperature function. 
*           -Read_Temperature() read function to read data from TMA102 sensor in C,K,F
*           -temperature_function() temperature pthread spawned calls this function which initalises the temperature queue intializes the timer parameters and starts a timer which logs temperature value periodically
*           -temperature_handler() Temperature timer handler which logs the temperature periodically by addding new temperature value to the temp queue.
* 
* References:
*           
_._._._._._._._._._._._._._._._._._._._._.*/




#ifndef TEMP_H
#define TEMP_H
/*Standard c libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

/*User defined libraries*/
#include "logger.h"
#include "i2c.h"

/*macros for temperature sensor*/
#define TMPSensor_Register 0x00
#define Minus_Correction 0xF800
#define Celsius 0.0625
#define KELVIN 273.5
#define FAREN 1.8
#define HEIT 32

/*global variable*/
int file;

/* -------------------------------*/
/**
 * @Synopsis temp_main() function is first initializes the I2C bus then calls the read_temperature function.
 *
 * @Param temp_value  used to return the temperature value to the callin function
 * @Param unit  used to indicate which is required by the calling function(Celcius, Kelvin or Farheniet)
 *
 * @Returns Exit status whether the temperature calculation was a success or not
 */
/* ---------------------------------*/
int temp_main(float *temp_value,int unit);

/* -------------------------------*/
/**
 * @Synopsis  read function to read data from TMA102 sensor in C,K,F
 *
 * @Param file
 * @Param unit used to indicate which is required by the calling function(Celcius, Kelvin or Farheniet)
 * @Param temp_value used to return the temperature value to the callin function
 *
 * @Returns Exit status whether the temperature calculation was a success or not
 */
/* ---------------------------------*/
int Read_Temperature(int file,int unit, float *temp_value);

/* -------------------------------*/
/**
 * @Synopsis temperature pthread spawned calls this function which initalises the temperature queue intializes the timer parameters and starts a timer which logs temperature value periodically
 *
 * @Param arg Nothing is passed to this function
 *
 * @Returns void as linked to phtread
 */
/* ---------------------------------*/
void *temperature_function(void *arg);


/* -------------------------------*/
/**
 * @Synopsis Temperature timer handler which logs the temperature periodically by addding new temperature value to the temp queue.
 *
 * @Param sv  used to pass the log file name.
 */
/* ---------------------------------*/
void temperature_handler(union sigval sv);


#endif
