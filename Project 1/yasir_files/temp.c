
#include "temp.h"
#include "i2c.h"
int val;
int temp_main(void)
{
  int unit=1;
  int result=I2C_init(&file,1);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  result = Read_Temperature(file,1);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = Read_Temperature(file,2);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Reading Failed!\n");
    return EXIT_FAILURE;
  } 
  result = Read_Temperature(file,3);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = Write_Configuration(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Write Configuration Failed!\n");
    return EXIT_FAILURE;
  }
 result = Read_TLow(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Read TLow Failed!\n");
    return EXIT_FAILURE;
  }
 result = Read_THigh(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor THigh Failed!\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


//read function to read data from TMA102 sensor in C,K,F
int Read_Temperature(int file,int unit)
{

  int result=I2C_Write_Byte(file,TMPSensor_Register);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  usleep(500);
  char buffer[1];
  result=I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  } 
	
  int temperature;
  temperature = ((buffer[0]) << 8) | (buffer[1]);
  temperature >>= 4;

  //correcting the signed bit
	if (temperature & (1 << 11))
  {
    temperature = temperature | Minus_Correction;
  }
  float  final_temperature= temperature * Celsius;
  if(unit == 1)
  {
    final_temperature= temperature * Celsius;
	  printf("Temp:%02fC  ", final_temperature);
  }
  else if(unit == 2)
  {
    float final_temperature_F = Temp_Conversion(final_temperature,unit);
    printf("Temp:%02fF  ", final_temperature_F);

  }
  else if(unit == 3)
  {
    float final_temperature_K = Temp_Conversion(final_temperature,unit);
    printf("Temp:%02fK  ", final_temperature_K);
 }
    usleep(10);
  return EXIT_SUCCESS;
}

int Write_Pointer_Reg(int file, uint8_t value)
{
	int result = I2C_Write_Byte(file,value);
  	if(result == EXIT_FAILURE)
  	{
    		perror("\nError: Failed to Write to Pointer!\n");
    		return EXIT_FAILURE;
  	}
}

int Read_TLow(int file)
{
  char buffer[1];	
  int result = I2C_Write_Byte(file,TMPSensor_TLow);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  int temperature=0;
  Write_Configuration(file);
  float temp = 0.0;

  if(val == 0)
  {
  	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 3;
	temp = temperature * Celsius;
  }
  else 
  {
	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 4; 
  	temp = temperature * Celsius ;
  }	
  printf("\nRead Value TLow is %f\n",temp); 
 
  return EXIT_SUCCESS;
}

int Read_THigh(int file)
{
  int result = I2C_Write_Byte(file,TMPSensor_THigh);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  uint8_t buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  float temp= 0.0;
  int temperature = 0;
  Write_Configuration(file);
  if(val == 0)
  {
  	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 3; 
 	temp = temperature * Celsius ;
  }
  else 
  {
	temperature = ((buffer[0]) << 8) | (buffer[1]);
  	temperature >>= 4; 	
  	temp = temperature * Celsius ;
  }	
  printf("\nRead Value THigh is %f\n",temp);
  return EXIT_SUCCESS;
}


int Write_Configuration(int file)
{
  int value =TMPSensor_Configuration;	
  int result=Write_Pointer_Reg(file,value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  }
  char buffer[1];
  result = I2C_Read_Word(file,buffer);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Read!\n");
    return EXIT_FAILURE;
  }
  unsigned char LSB =buffer[1];
  LSB = LSB & Mask;
  if(LSB =0x10)
  {
  	val =1;
  }
  else val =0;

  return EXIT_SUCCESS;
}
