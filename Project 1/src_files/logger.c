#include "logger.h"

void *logging_thread(void *arg)
{
  thread_struct *logging_thread = (thread_struct *)arg;
  /*light queue attribute settings*/
  mqd_t light_discriptor;
  struct mq_attr attribute;
  attribute.mq_maxmsg = 10;
  attribute.mq_msgsize = sizeof(msg_struct);
  light_discriptor = mq_open ("/light_queue", O_RDWR | O_CREAT, 0666, &attribute);
  if (light_discriptor < 0)
  {
    printf("message queue error\n");
  }
  else
  {
    printf("Successful in creating message queue\n");
  }

  /*temperature queue attribute setting*/
  mqd_t temperature_discriptor;
  temperature_discriptor = mq_open ("/temp_queue", O_RDWR | O_CREAT, 0666, &attribute);
  
  if (temperature_discriptor < 0)
  {
    printf("message queue error\n");
  }
  else
  {
    printf("Successful in creating message queue\n");
  }
  while(1)
  {
    if(light_flag)
    {
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      light_flag = 0;
      if(mq_receive(light_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
      {
        printf("Error receiving msg to  queue\n");
      }
      else
      {
        printf("received from queue\n");
      }
      logging_function(getppid(),getpid(),syscall(SYS_gettid),logging_thread->log_file,"Timer event light handler",msg);
    }
    if(temperature_flag)
    {
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      temperature_flag = 0;
      if(mq_receive(temperature_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
      {
        printf("Error receiving msg to  queue\n");
      }
      else
      {
        printf("received from queue\n");
      }
      temperature_flag = 0;
      logging_function(getppid(),getpid(),syscall(SYS_gettid),logging_thread->log_file,"Timer event temperature handler",msg);
    }
  }

}

void logging_function(int parent_id, int pthread_id, int thread_id, char *file_name,char* ip_str,msg_struct *msg)
{
  FILE *file_ptr = fopen(file_name,"a");
  struct timeval curr_time;
  gettimeofday(&curr_time, NULL);
  fprintf(file_ptr,"[Timestamp: %ld] Parent ID:%d, Posix thread ID:%d, Linux Thread ID:%d, ",(curr_time.tv_sec),parent_id, pthread_id, thread_id);
  fprintf(file_ptr,"%s",ip_str);
  if(msg != NULL)
    fprintf(file_ptr,"Thread:%s\tsensor_value:%.2f%c\n",msg->thread_name,msg->sensor_value,msg->unit);
  else
    fprintf(file_ptr,"\n");
  fflush(file_ptr);
  fclose(file_ptr);
}


