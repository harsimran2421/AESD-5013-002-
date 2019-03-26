
#include "temp.h"
#include "i2c.h"

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
  char buf[1];
  result=I2C_Read_Word(file,buf);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Failed to Write!\n");
    return EXIT_FAILURE;
  } 
	
  int temperature;
	temperature = ((buf[0]) << 8) | (buf[1]);
	temperature >>= 4;
  //printf("\nTemperature =%d\n",temperature);
  
  //correcting the signed bit
	if (temperature & (1 << 11))
  {
    temperature = temperature | Minus_Correction;
  }
  float final_temperature = temperature * Celsius ;
  
  if(unit == 1)
  {
    final_temperature= temperature * Celsius;
	  printf("/nTemp:%02fC  ", final_temperature);
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
