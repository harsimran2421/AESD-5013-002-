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
  trigger.it_interval.tv_sec=3;
  trigger.it_interval.tv_nsec=0;
  timer_settime(timer_id,0, &trigger, NULL);
  while(1);
}

void temperature_handler(union sigval sv)
{
  static int i;
  char *file_name = sv.sival_ptr;
  FILE *file_ptr;
  printf("\nlogging temperature %d\n",i++);
  msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
  memset(msg->thread_name,'\0',sizeof(msg->thread_name));
  memcpy(msg->thread_name,"temperature",strlen("temperature"));
  msg->sensor_value = 8.2;
  msg->unit = 'C';

  /*send light sensor value to light queue*/
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

