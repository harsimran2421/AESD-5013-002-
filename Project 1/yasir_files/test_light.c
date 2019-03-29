#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "light.h"
#define status EXIT_FAILURE
void MY_ASSERT_NOT_EQUAL(int result);


int Test_I2C_init(int file)
{
	printf("\nTest I2C Init:");
	int result = I2C_init(&file,2);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Turn_on_light_sensor(int file)
{
	printf("\nTest Turn Sensor ON:");
	I2C_init(&file,2);	
	int result = Turn_on_Light_sensor(file);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Check_PowerUp(int file)
{
	printf("\nTest Check Power Up:");
	I2C_init(&file,2);
	Turn_on_Light_sensor(file);
	int result = Check_PowerUp(file);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Read_Sensor_ID(int file,uint8_t SensorID)
{
	printf("\nTest Read Sensor ID:");
	I2C_init(&file,2);
	Turn_on_Light_sensor(file);
	int result = Read_Sensor_ID(file,&SensorID);
	MY_ASSERT_NOT_EQUAL(result);
}
int Test_Light_Sensor(int file)
{
	printf("\nTest Light Sensor:");
	I2C_init(&file,2);
	Turn_on_Light_sensor(file);
	int result = Read_Light_Sensor(file);
	MY_ASSERT_NOT_EQUAL(result);
}
int Test_Write_Interrupt(int file, uint8_t SensorID)
{
	printf("\nTest Write Interrupt:");
	I2C_init(&file,2);
	Turn_on_Light_sensor(file);
	int result = Write_Interrupt(file,&SensorID);
	MY_ASSERT_NOT_EQUAL(result);
}
int Test_Read_Interrupt(int file)
{
	printf("\nTest Read Interrupt:");
	I2C_init(&file,2);
	Turn_on_Light_sensor(file);
	int result = Read_Interrupt(file);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Status(int file)
{
	printf("\nTest Status:");
	I2C_init(&file,2);
	int result = State(file,Lux_Value);
	MY_ASSERT_NOT_EQUAL(result);
}


void MY_ASSERT_NOT_EQUAL(int result)
{
	if(result != status)
	{
		printf("Pass!\n");
		return;
	}
	else printf("Fail!");	
}

int main()
{
	int file;
	uint8_t SensorID;
	uint8_t in_value;

	Test_I2C_init(file);
	Test_Turn_on_light_sensor(file);
	Test_Check_PowerUp(file);
	Test_Read_Sensor_ID(file,SensorID);
	Test_Light_Sensor(file);
	Test_Status(file);
	Test_Write_Interrupt(file,SensorID);
	Test_Read_Interrupt(file);
}

