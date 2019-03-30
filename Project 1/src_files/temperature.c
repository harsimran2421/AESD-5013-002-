#include "temperature.h"

mqd_t temp_discriptor;

void *temperature_function(void *arg)
{
  struct mq_attr attribute;
  attribute.mq_maxmsg = 10;
  attribute.mq_msgsize = sizeof(msg_struct);
  attribute.mq_flags = 0;
  attribute.mq_curmsgs = 0;

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
  timer_settime(timer_id,0, &trigger, NULL);
  while(exit_flag != 1);
  timer_delete(timer_id);
  mq_close(temp_discriptor);
}

void temperature_handler(union sigval sv)
{
  pthread_cond_broadcast(&temp_thread_cond);
  int unit = 1;
  char *file_name = sv.sival_ptr;
  FILE *file_ptr;
  float temp_value;
  int result = temp_main(&temp_value, unit);
  if(result == EXIT_FAILURE)
  { 
    printf("\nError: Failed to Run Temperature Sensor!\n");
    return;
  }
  else
  {
//  printf("\nlogging temperature %d\n",i++);
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
  /*send light sensor value to light queue*/
  printf("temperature value:%0.2f\n",temp_value);
  if(mq_send(temp_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
  {
    printf("Error sending to temperature queue\n");
    mq_close(temp_discriptor);
  }
  else
  {
    printf("Sent to temperature queue\n");
    temperature_flag = 1;
  }
  }
}


int temp_main(float *temp_value, int unit)
{
  pthread_mutex_lock(&bus_lock);
  int result=I2C_init(&file,1);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    pthread_mutex_lock(&bus_lock);
    return EXIT_FAILURE;
  }
  result = Read_Temperature(file,unit,temp_value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Reading Failed!\n");
    pthread_mutex_lock(&bus_lock);
    return EXIT_FAILURE;
  }

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
	//  printf("/nTemp:%02fC  ", final_temperature);
    *temp_value = final_temperature;
  }
  else if(unit == 2)
  {
    float final_temperature_F = Temp_Conversion(final_temperature,unit);
  //  printf("Temp:%02fF  ", final_temperature_F);
    *temp_value = final_temperature_F;

  }
  else if(unit == 3)
  {
    float final_temperature_K = Temp_Conversion(final_temperature,unit);
  //  printf("Temp:%02fK  ", final_temperature_K);
    *temp_value = final_temperature_K;
 }
  return EXIT_SUCCESS;
}
