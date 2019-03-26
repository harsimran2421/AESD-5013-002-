
/*
 *References-https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/
 *
 */

#include "logger.h"
#include "light.h"
#include "temperature.h"


void main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("\nPlease input the log file name\n");
    exit(0);
  }
  pthread_t light_task, temperature_task, logging_task;
  
  /*Mutex lock for I2C bus synchronisation*/
  if(pthread_mutex_init(&bus_lock,NULL)!= 0)
  {
    printf("\nBus lock mutex creation failed\n");

  }

  thread_struct *thread_input = (thread_struct *)malloc(sizeof(struct thread_content));
  thread_input->log_file = argv[1];
  if(!pthread_create(&light_task, NULL, light_function, (void *)thread_input))
  {
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"LIGHT Thread created Successfully\n",NULL);
  }
  else
  {
    printf("LIGHT read Thread creation failed\n");
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"LIGHT read Thread creation failed",NULL);
  }
  if(!pthread_create (&temperature_task, NULL, temperature_function, (void*)thread_input))
  {
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"temperature thread created successfully",NULL);
  }
  else
  {
    printf("temperature Thread creation failed\n");
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"temperature Thread creation failed",NULL);
  }

  if(!pthread_create (&logging_task, NULL, logging_thread, (void*)thread_input))
  {
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file," logging thread created successfully",NULL);
  }
  else
  {
    printf("logging Thread creation failed\n");
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"logging Thread creation failed",NULL);
  }
  pthread_join(temperature_task, NULL);
  pthread_join(light_task, NULL);
  pthread_join(logging_task,NULL);
}
