#include "logger.h"
#include "temperature.h"
#include "light.h"


int intial_test(char *filename)
{
  int result = temp_test();
  if(result == EXIT_FAILURE)
  { 
      printf("\nTemperature Sensor disconnected!\n");
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"temperature",strlen("temperature"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"ALERT",strlen("ALERT"));
      msg->unit = '\0';
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"Temperature sensor not functional",msg);
      return EXIT_FAILURE;
  }
  else if(result == EXIT_SUCCESS)
  {
      printf("\nTemperature Sensor Successfully connected\n");
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"temperature",strlen("temperature"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"INFO",strlen("INFO"));
      msg->unit = '\0';
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"Temperature sensor working",msg);
  }
  result = light_test();
  if(result == EXIT_FAILURE)
  { 
      printf("\nlightSensor disconnected!\n");
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"ALERT",strlen("ALERT"));
      msg->unit = '\0';
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"light sensor not functional",msg);
      return EXIT_FAILURE;
  }
  else if(result == EXIT_SUCCESS)
  {
      printf("\nLight sensor successfully connected\n");
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"MAIN",strlen("MAIN"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"INFO",strlen("INFO"));
      msg->unit = '\0';
      logging_function(getppid(),getpid(),syscall(SYS_gettid),filename,"Light sensor working",msg);
  }
  return EXIT_SUCCESS;
}

