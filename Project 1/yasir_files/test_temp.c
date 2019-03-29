#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "temp.h"
#define status EXIT_FAILURE
void MY_ASSERT_NOT_EQUAL(int result);


int Test_I2C_init(int file)
{
	printf("\nTest I2C Init:");
	int result = I2C_init(&file,1);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Read_Sensor_Celsius(int file)
{
	printf("\nTest Read Celsius:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,1);
	MY_ASSERT_NOT_EQUAL(result);
}


int Test_Read_Sensor_Kelvin(int file)
{
	printf("\nTest Read Kelvin:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,2);
	MY_ASSERT_NOT_EQUAL(result);
}


int Test_Read_Sensor_Farenheit(int file)
{
	printf("\nTest Read Farenheit:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,3);
	MY_ASSERT_NOT_EQUAL(result);
}
int Test_Write_Configuration(int file)
{
	printf("\nTest Write Configuration");
	I2C_init(&file,2);
	int result = Write_Configuration(file);
        MY_ASSERT_NOT_EQUAL(result);
}
int Test_Read_TLow(int file)
{
	printf("\nTest Read Sensor TLow:");
	I2C_init(&file,1);
	int result = Read_TLow(file);
	MY_ASSERT_NOT_EQUAL(result);
}
int Test_Read_THigh(int file)
{
	printf("\nTest Read Sensor THigh:");
	I2C_init(&file,1);
	int result = Read_THigh(file);
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

	Test_I2C_init(file);
	Test_Read_Sensor_Celsius(file);
	Test_Read_Sensor_Kelvin(file);	
	Test_Read_Sensor_Farenheit(file);
	Test_Write_Configuration(file);
	Test_Read_TLow(file);
	Test_Read_THigh(file);
}

