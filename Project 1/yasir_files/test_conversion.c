#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "temp.h"
#include "i2c.h"

#define status EXIT_FAILURE
void MY_ASSERT_NOT_EQUAL(int result);

int Test_I2C_init(int file)
{
	printf("\nTest I2C Init:");
	int result = I2C_init(&file,1);
	MY_ASSERT_NOT_EQUAL(result);
}

int Test_Read_Temperature_Celsius(int file)
{
	printf("\nTest Temperature in Celsius:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,1);
	MY_ASSERT_NOT_EQUAL(result);
}


int Test_Read_Temperature_Kelvin(int file)
{
	printf("\nTest Temperature in Kelvin:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,2);
	MY_ASSERT_NOT_EQUAL(result);
}


int Test_Read_Temperature_Farenheit(int file)
{
	printf("\nTest Temperature in Farenheit:");
	I2C_init(&file,1);	
	int result = Read_Temperature(file,3);
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
	Test_Read_Temperature_Celsius(file);
	Test_Read_Temperature_Kelvin(file);	
	Test_Read_Temperature_Farenheit(file);
  return 0;
}
