/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : temperature.h
* Creation Date : 21-03-2019
* Last Modified : Wed 31 March 2019 00:02:56 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: Source file contataining function defintion for reading from the temperature sensor
* 
* Functions:
*           -temp_main() function is first initializes the I2C bus then calls the read_temperature function. 
*           -Read_Temperature() read function to read data from TMA102 sensor in C,K,F
*           -temperature_function() temperature pthread spawned calls this function which initalises the temperature queue intializes the timer parameters and starts a timer which logs temperature value periodically
*           -temperature_handler() Temperature timer handler which logs the temperature periodically by addding new temperature value to the temp queue.
* 
* References:
*           
_._._._._._._._._._._._._._._._._._._._._.*/



/*user defined library*/
#include "temperature.h"

mqd_t temp_discriptor;

void *temperature_function(void *arg)
{
  struct mq_attr attribute;
  attribute.mq_maxmsg = 10;
  attribute.mq_msgsize = sizeof(msg_struct);
  attribute.mq_flags = 0;
  attribute.mq_curmsgs = 0;

  /*create a queue for transferring temperature value to logger task*/
  temp_discriptor = mq_open ("/temp_queue", O_RDWR | O_CREAT, 0666, &attribute);
  if (temp_discriptor < 0)
  {
    printf("temperature message queue error\n");
  }
  else
  {
    printf("temperature message queue created successfully\n");
  }

  timer_t timer_id;
  thread_struct *light_thread = (thread_struct *)arg;
  struct sigevent sev;
  memset(&sev,0,sizeof(struct sigevent));
  struct itimerspec trigger;
  memset(&trigger,0,sizeof(struct itimerspec));

/*set timer event characteristics*/
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = &temperature_handler;
  sev.sigev_value.sival_ptr = light_thread->log_file;
  timer_create(CLOCK_REALTIME, &sev, &timer_id);

/*set timer capabilities*/
  trigger.it_value.tv_sec=1;
  trigger.it_value.tv_nsec=0;
  trigger.it_interval.tv_sec=1;
  trigger.it_interval.tv_nsec=0;
  /*enable timer*/
  timer_settime(timer_id,0, &trigger, NULL);
  /*wait for the exit flag*/
  while(exit_flag != 1);
  timer_delete(timer_id);
  /*close the queue before exit*/
  mq_close(temp_discriptor);
}

void temperature_handler(union sigval sv)
{
  /*For heartbeat implementation*/
  pthread_cond_broadcast(&temp_thread_cond);
  int unit = 1;
  char *file_name = sv.sival_ptr;
  FILE *file_ptr;
  float temp_value;
  static int error_status;
  /*read temperature periodically*/
  int result = temp_main(&temp_value, unit);
  if(result == EXIT_FAILURE)
  { 
    error_status++;
    /*condition when sensor is plugged out*/
    if(error_status == 1)
    {
      int light_led_status = 1;
      /*turn an led on to indicate sensor failure*/
      led_control(RED,light_led_status);
      printf("\nTemperature Sensor disconnected!\n");
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      memset(msg->thread_name,'\0',sizeof(msg->thread_name));
      memcpy(msg->thread_name,"temperature",strlen("temperature"));
      memset(msg->level,'\0',sizeof(msg->level));
      memcpy(msg->level,"ALERT",strlen("ALERT"));
      msg->unit = '\0';
      /*send alert message to the logger task*/
      if(mq_send(temp_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
      {
        printf("Error sending to temperature queue\n");
        mq_close(temp_discriptor);
      }
      else
      {
        temperature_flag = 1;
      }
    }
  }
  /*condition is sensor is read successfully*/
  else if(result == EXIT_SUCCESS)
  {
    if(error_status != 0)
    {
      int light_led_status = 0;
      led_control(RED,light_led_status);
    }
    error_status = 0;
    msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
    memset(msg->thread_name,'\0',sizeof(msg->thread_name));
    memcpy(msg->thread_name,"temperature",strlen("temperature"));
    msg->sensor_value = temp_value;
    memset(msg->level,'\0',sizeof(msg->level));
    memcpy(msg->level,"DATA",strlen("DATA"));
    if(unit == 1)
      msg->unit = 'C';
    else if(unit == 2)
      msg->unit = 'K';
    else if(unit == 3)
      msg->unit = 'F';
    printf("Temperature value:%0.2f%c\n",msg->sensor_value,msg->unit);
    /*send data to the logger task*/
    if(mq_send(temp_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
    {
      printf("Error sending to temperature queue\n");
      mq_close(temp_discriptor);
    }
    else
    {
      temperature_flag = 1;
    }
  }
  /*print error if sensor is disconnected*/
  if(error_status > 0 && error_status%2 == 0)
  {
    printf("\ntemperature Sensor disconnected\n");
  }
}

int temp_main(float *temp_value, int unit)
{
  /*lock mutex for synchronised I2C access*/
  pthread_mutex_lock(&bus_lock);
  /*intialize the I2C bus*/
  int result=I2C_init(&file,1);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor Initialization Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  result = Read_Temperature(file,unit,temp_value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor Reading Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  /*return the I2C bus mutex*/
  pthread_mutex_unlock(&bus_lock);
  return EXIT_SUCCESS;
}

/*to test the temperature sensor*/
int temp_test()
{
  float temp_value;
  /*lock mutex for synchronised I2C access*/
  pthread_mutex_lock(&bus_lock);
  /*intialize the I2C bus*/
  int result=I2C_init(&file,1);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor Initialization Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  result = Read_Temperature(file,1,&temp_value);
  if(result == EXIT_FAILURE)
  {
    perror("\nError: Sensor Reading Failed!\n");
    pthread_mutex_unlock(&bus_lock);
    return EXIT_FAILURE;
  }
  /*return the I2C bus mutex*/
  pthread_mutex_unlock(&bus_lock);
  return EXIT_SUCCESS;
}


//read function to read data from TMA102 sensor in C,K,F
int Read_Temperature(int file,int unit, float *temp_value)
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
  
  /*correcting the signed bit*/
	if (temperature & (1 << 11))
  {
    temperature = temperature | Minus_Correction;
  }
  float final_temperature = temperature * Celsius ;
  
  if(unit == 1)
  {
    final_temperature= temperature * Celsius;
    *temp_value = final_temperature;
  }
  else if(unit == 2)
  {
    float final_temperature_F = Temp_Conversion(final_temperature,unit);
    *temp_value = final_temperature_F;

  }
  else if(unit == 3)
  {
    float final_temperature_K = Temp_Conversion(final_temperature,unit);
    *temp_value = final_temperature_K;
 }
  return EXIT_SUCCESS;
}
