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
#define TMPSensor_THigh 0x03
#define Mask 0b00010000
#define Minus_Correction 0xF800
#define Celsius 0.0625
#define KELVIN 273.5
#define FAREN 1.8
#define HEIT 32
#define TMPSensor_Conversion_8 0xC0
#define TMPSensor_Conversion_4 0x10
#define TMPSensor_Conversion_1 0x40
#define EM_Set 0x10
#define ShutDown_Set 0x6100
#define ShutDown_Clear 0x6000
#define Fault_Bits_4 0x7800
#define Fault_Bits_1 0x6000
#define Resolution 0x6000
//global variable 
int file;

int temp_main(void);
//read function to read data from TMA102 sensor in C,K,F
int Read_Temperature(int file,int unit);
int Write_pointer_reg(int file, uint8_t value);
int Read_TLow(int file);
int Read_THigh(int file);
int Read_Resolution(int file);
int Read_Fault_Bits(int file);
int Set_ShutDown(int file);
int Clear_ShutDown(int file);
int Set_EM(int file);
int Set_Conversion(int file);
int Write_Configuration(int file);
#endif
