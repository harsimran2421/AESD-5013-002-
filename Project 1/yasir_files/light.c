/*APDS sensor*/
//include headers
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <math.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c-dev.h>
#include<unistd.h>

//user defined header files
#include "i2c.h"
#include "light.h"

//global Variables
int file;
float Lux_Value;

int Light_main()
{
  //2 is for Sensor: Light
  int result = I2C_init(&file,2);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  result = Turn_on_Light_sensor(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  
  result = Check_PowerUp(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  //usleep(500);

  result =Read_Light_Sensor(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  printf("\nLux is %f\n",Lux_Value);
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
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&LSB);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
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
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&MSB);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  Final = (MSB<<8)|(LSB);
  //printf("\nValue is %d\n",Final);
  return Final;
}

//function to read lux values
int Read_Light_Sensor(int file)
{
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
return EXIT_SUCCESS;

}

//function to check sensor is working by power up
int Turn_on_Light_sensor(int file)
{
  int result = write_control(file,Power_Up);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  result = write_timing(file,0x12);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
}

//function to see if power up is working
int Check_PowerUp(int file)
{
  int result = I2C_Write_Byte(file,Command_Control);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  uint8_t buf[1];
  result = I2C_Read_Word(file,buf);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  int value = buf[0];

  if(value = Power_Up)
  {
    printf("\nSensor Checkup Successfully!\n");
  }
  else 
  {
    printf("\nFailed!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
