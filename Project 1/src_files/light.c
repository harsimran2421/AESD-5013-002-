
#include "light.h"


int file;
mqd_t ser_discriptor;


void *light_function(void *arg)
{
  struct mq_attr attribute;
  attribute.mq_maxmsg = 10;
  attribute.mq_msgsize = sizeof(msg_struct);
  attribute.mq_flags = 0;
  attribute.mq_curmsgs = 0;

  ser_discriptor = mq_open ("/light_queue", O_RDWR | O_CREAT, 0666, &attribute);
  if (ser_discriptor < 0)
  {
    printf("light message queue error\n");
  }
  else
  {
    printf("light message queue created successfully\n");
  }

  timer_t timer_id;
  thread_struct *light_thread = (thread_struct *)arg;
  struct sigevent sev;
  memset(&sev,0,sizeof(struct sigevent));
  struct itimerspec trigger;
  memset(&trigger,0,sizeof(struct itimerspec));

/*set timer event characteristics*/
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = &light_handler;
  sev.sigev_value.sival_ptr = light_thread->log_file;
  timer_create(CLOCK_REALTIME, &sev, &timer_id);

/*set timer capabilities*/
  trigger.it_value.tv_sec=6;
  trigger.it_value.tv_nsec=0;
  trigger.it_interval.tv_sec=6;
  trigger.it_interval.tv_nsec=0;
  timer_settime(timer_id,0, &trigger, NULL);
  while(1);
  mq_close(ser_discriptor);
}

void light_handler(union sigval sv)
{
  static int i;
  char *file_name = sv.sival_ptr;
  FILE *file_ptr;
  float light_value = 0;
  printf("\nlogging light value:%d\n", i++);
  int result = Light_main(&light_value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError light sensor\n");
  }
  else
  {
  printf("light value is: %0.2f\n",light_value);
  msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
  memset(msg->thread_name,'\0',sizeof(msg->thread_name));
  memcpy(msg->thread_name,"light",strlen("light"));
  msg->sensor_value = light_value;
  memset(msg->level,'\0',sizeof(msg->level));
  memcpy(msg->level,"DATA",strlen("DATA"));

  msg->unit = 'L';

  /*send light sensor value to light queue*/
  if(mq_send(ser_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
  {
    printf("Error sending to light queue\n");
    mq_close(ser_discriptor);
  }
  else
  {
    printf("Sent to light queue\n");
    light_flag = 1;
  }
  }
}

int Light_main(float *light_value)
{
  //2 is for Sensor: Light
  pthread_mutex_lock(&bus_lock); 
  int result = I2C_init(&file,2);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    pthread_mutex_lock(&bus_lock);
    return EXIT_FAILURE;
  }
  result = Turn_on_Light_sensor(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    pthread_mutex_lock(&bus_lock);
    return EXIT_FAILURE;
  }
  
  result = Check_PowerUp(file);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    pthread_mutex_lock(&bus_lock);
    return EXIT_FAILURE;
  }
  //usleep(500);

  result =Read_Light_Sensor(file, light_value);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    pthread_mutex_lock(&bus_lock);
    return EXIT_FAILURE;
  }
  pthread_mutex_unlock(&bus_lock); 
  return EXIT_SUCCESS;
}

uint16_t Read_Data(int file, int flag)
{
  uint8_t LSB,MSB;
  uint16_t Final;
  uint8_t address;
  if(flag == 1)
  {
    address =Command_Control | Data_Low0;
  }
  else if(flag ==2)
  {
    address =Command_Control | Data_Low1;
  }
    int result = I2C_Write_Byte(file,address);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&LSB);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
   if(flag == 1)
  {
    address =Command_Control | Data_High0;
  }
  else if(flag ==2)
  {
    address =Command_Control | Data_High1;
  }
  result = I2C_Write_Byte(file,address);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  result = I2C_Read_Byte_Data(file,&MSB);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: SensorII Reading Failed!\n");
    return EXIT_FAILURE;
  }
  Final = (MSB<<8)|(LSB);
  //printf("\nValue is %d\n",Final);
  return Final;
}

//function to read lux values
int Read_Light_Sensor(int file, float *light_value)
{ float Lux_Value = 0;
  float data0 =(float)Read_Data(file,1);
  float data1 =(float)Read_Data(file,2);  
	if(data0 == 0)
	{
		perror("Error:Process Failed!");
		return EXIT_FAILURE;
	}
	
	float ADC= data1/data0;
  float power = pow(ADC,1.4);
	if(0 < ADC < 0.50)
	{
		Lux_Value = (0.0304 * data0) - (0.062 * data0 * power);
	}
	else if(0.50 < ADC < 0.61)
	{
		Lux_Value = (0.0224 * data0) - (0.031 * data1);
	}
	else if(0.61 < ADC < 0.80)
	{
		Lux_Value = (0.0128 * data0) - (0.0153 * data1);
	}
	else if(0.80 < ADC < 1.30)
	{
		Lux_Value = (0.00146 * data0) - (0.00112 * data1);
	}
	else
	{
		Lux_Value = 0;
	}
  *light_value = Lux_Value;
return EXIT_SUCCESS;
}

//function to check sensor is working by power up
int Turn_on_Light_sensor(int file)
{
  int result = write_control(file,Power_Up);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  result = write_timing(file,0x12);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
}

//function to see if power up is working
int Check_PowerUp(int file)
{
  int result = I2C_Write_Byte(file,Command_Control);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  uint8_t buf[1];
  result = I2C_Read_Word(file,buf);
  if(result == EXIT_FAILURE)
  {
    printf("\nError: Sensor Initialization Failed!\n");
    return EXIT_FAILURE;
  }
  int value = buf[0];

  if(value = Power_Up)
  {
    printf("\nSensor Checkup Successfully!\n");
  }
  else 
  {
    printf("\nFailed!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

