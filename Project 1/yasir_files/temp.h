#ifndef TEMP_H
#define TEMP_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "i2c.h"

//macros for temperature sensor
#define TMPSensor_Register 0x00
#define TMPSensor_Configuration 0x01
#define TMPSensor_TLow 0x02
#define TMPSensor_THigh 0x02
#define Mask 0b00010000
#define Minus_Correction 0xF800
#define Celsius 0.0625
#define KELVIN 273.5
#define FAREN 1.8
#define HEIT 32

//global variable 
int file;

int temp_main(void);
//read function to read data from TMA102 sensor in C,K,F
int Read_Temperature(int file,int unit);
int Read_TLow(int file);
int Read_THigh(int file);
int Write_Configuration(int file);
#endif
