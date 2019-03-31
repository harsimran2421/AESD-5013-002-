/*Unit Test to test various functions related to Temperature
 * Author: Harsimransingh Bindra & Yasir Aslam Shah
 * Advance Embedded System Design
 * Spring 2019
 * Definition: Test module to test various functionality for teh temperarture sensor.Includes the following functions:
 *Test_I2C_init(),Test_Read_Sensor_Celsius(),Test_Read_Sensor_Kelvin(),Test_Read_Sensor_Farenheit(),Test_Read_TLow(),Test_Read_THigh(int file);
int Test_Read_Resolution(int file);
int Test_Read_Fault_Bits(int file);
int Test_Clear_ShutDown(int file):
int Test_Set_ShutDown(int file):
int Test_Set_EM(int file);
int Test_Set_Conversion(int file);
int Test_Write_Configuration(int file);
void MY_ASSERT_NOT_EQUAL(int result);


 * */

//Standard Header files
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

//User Defined Header files
#include "temp.h"

//Macros
#define status EXIT_FAILURE

//Function declaration
int Test_I2C_init(int file);
int Test_Read_Sensor_Celsius(int file);
int Test_Read_Sensor_Kelvin(int file);
int Test_Read_Sensor_Farenheit(int file);
int Test_Read_TLow(int file);
int Test_Read_THigh(int file);
int Test_Read_Resolution(int file);
int Test_Read_Fault_Bits(int file);
int Test_Clear_ShutDown(int file):
int Test_Set_ShutDown(int file):
int Test_Set_EM(int file);
int Test_Set_Conversion(int file);
int Test_Write_Configuration(int file);
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

int Test_Read_Resolution(int file)
{
	printf("\nTest Read Resolution:");
	I2C_init(&file,1);
	int result = Read_Resolution(file);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Read_Fault_Bits(int file)
{
	printf("\nTest Read Fault Bits:");
	I2C_init(&file,1);
	int result = Read_Fault_Bits(file);
	MY_ASSERT_NOT_EQUAL(result);
}
int Test_Clear_ShutDown(int file)
{
	printf("\nTest ShutDown Disable:");
	I2C_init(&file,1);
	int result = Clear_ShutDown(file);
	MY_ASSERT_NOT_EQUAL(result);
}
int Test_Set_ShutDown(int file)
{
	printf("\nTest Read Set ShutDown:");
	I2C_init(&file,1);
	int result = Set_ShutDown(file);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Set_EM(int file)
{
	printf("\nTest Read Set Extended Mode:");
	I2C_init(&file,1);
	int result = Set_EM(file);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Set_Conversion(int file)
{
	printf("\nTest Read Set Conversion:");
	I2C_init(&file,1);
	int result = Set_Conversion(file);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Write_Configuration(int file)
{
	printf("\nTest Write Configuration:");
	I2C_init(&file,1);
	int result = Write_Configuration(file);
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
	Test_Read_Resolution(file);
	Test_Read_Fault_Bits(file);
	Test_Clear_ShutDown(file);
	Test_Set_ShutDown(file);
	Test_Set_EM(file);
	Test_Set_Conversion(file);
	Test_Write_Configuration(file);
}
