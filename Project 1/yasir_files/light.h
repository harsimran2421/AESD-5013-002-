#ifndef LIGHT_H
#define LIGH_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

#include "i2c.h"


//macros for Light Sensor APDS 9301
#define Power_Up 0x03
#define Command_Control 0x80
#define Command_Reg 0x00
#define Timing_Reg 0x01
#define Data_Low0 0x0c
#define Data_High0 0x0D 
#define Data_Low1 0x0E
#define Data_High1 0x0F

//global variables
int file;
float Lux_Value;

/*functions*/
//init function for light sensor to initialise and read
int Light_main();

//read function to read LUX Low and High Values 
uint16_t Read_Data(int file, int flag);

//function to read lux value
int Read_Light_Sensor(int file);

//function to check sensor is working by power up
int Turn_on_Light_sensor(int file);

//function to see if power up is working
int Check_PowerUp(int file);
#endif
