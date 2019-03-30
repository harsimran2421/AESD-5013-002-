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
#define Sensor_ID 0x0A
#define Set_Gain 0x12
#define Interrupt_Control_reg_Disable 0x06
#define Interrupt_Control_reg_Enable 0x06
#define Threshold_LL 0x02
#define Threshold_LH 0x03
#define Threshold_HL 0x04
#define Threshold_HH 0x05


//global variables
int file;
float Lux_Value;

/*functions*/
//init function for light sensor to initialise and read
int Light_main();
int State(int file, int Lux);
//read function to read LUX Low and High Values 
uint16_t Read_Data(int file, int flag);

//function to read lux value
int Read_Light_Sensor(int file);

//function to check sensor is working by power up
int Turn_on_Light_sensor(int file);

//function to see if power up is working
int Check_PowerUp(int file);
//function to read SensorID
int Read_Sensor_ID(int file,uint8_t *data);
int Read_Interrupt(int file);
int Write_Interrupt(int file, uint8_t *in_value);  
int Disable_Interrupt_Control_Register(int file);
int Enable_Interrupt_Control_Register(int file);
#endif
