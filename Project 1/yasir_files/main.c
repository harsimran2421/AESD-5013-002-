#include "temp.h"
#include "light.h"
#include "i2c.h"

int main()
{
  int result;
  result = temp_main();
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Failed to Run Temperature Sensor!\n");
    return EXIT_FAILURE;
  }
  usleep(1000);
  result = Light_main();
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Failed to Run Light Sensor!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
