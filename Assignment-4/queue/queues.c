#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <memory.h>
#include <unistd.h>

typedef struct{
    char string[50];
    int string_length;
    int led_control;
}msg_struct; 


mqd_t ser_discriptor, cli_discriptor;

void intHandler(int dummy) {
  FILE *file_ptr=fopen("queues.log","a");
  fprintf(file_ptr,"CTRL + C SIGNAL CAUGHT!!!!");
  fclose(file_ptr);
  mq_unlink("/one_queue");
  mq_close(cli_discriptor);
  mq_close(ser_discriptor);
  exit(0);
}

void logging_function(int parent_id, int pthread_id, char *file_name,char *status,msg_struct* ip_str)
{
  FILE *file_ptr = fopen(file_name,"a");
  struct timeval curr_time;
  gettimeofday(&curr_time, NULL);
  fprintf(file_ptr,"[Timestamp: %ld us] Parent ID:%d, Posix thread ID:%d",curr_time.tv_usec,parent_id, pthread_id);
  fprintf(file_ptr,"%s:%s LED control:%d\n",status,ip_str->string,ip_str->led_control);
  fflush(file_ptr);
  fclose(file_ptr);
}

void status_function(int parent_id, int pthread_id, char *file_name,char *status)
{
  FILE *file_ptr = fopen(file_name,"a");
  struct timeval curr_time;
  gettimeofday(&curr_time, NULL);
  fprintf(file_ptr,"[Timestamp: %ld us] Parent ID:%d, Posix thread ID:%d",curr_time.tv_usec,parent_id, pthread_id);
  fprintf(file_ptr,"%s\n",status);
  fflush(file_ptr);
  fclose(file_ptr);
}


void main()
{
  signal(SIGINT, intHandler);
  pid_t process_id;
  process_id = fork();
  if(process_id == 0)
  {
    sleep(1);
    struct mq_attr attribute;
    msg_struct *src_ptr = (msg_struct *)malloc(sizeof(msg_struct));
    msg_struct *dst_ptr = (msg_struct *)malloc(sizeof(msg_struct));
    attribute.mq_maxmsg = 10;
    attribute.mq_msgsize = sizeof(msg_struct);
    attribute.mq_flags = 0;
    attribute.mq_curmsgs = 0;

    ser_discriptor = mq_open ("/one_queue", O_RDWR | O_CREAT, 0666, &attribute);
    if (ser_discriptor < 0)
    {
      printf("message queue error\n");
      status_function(getppid(),getpid(),"queues.log","Error creating message queue"); 
    }
    else
    {
      printf("Successful in creating message queue\n");
      status_function(getppid(),getpid(),"queues.log","Successful in creating message queue"); 
    }
    for(int i = 0; i< 10; i++)
    {
      src_ptr->led_control ^= 1;
      sprintf(src_ptr->string, "From parent to child msg number:%d\n",i);
      src_ptr->string_length = strlen(src_ptr->string);

      if(mq_send(ser_discriptor,(char *)src_ptr,sizeof(msg_struct),0) < 0)
      {
        printf("Error sending to  queue\n");
        status_function(getppid(),getpid(),"queues.log","Error sending to message queue 1"); 
      }
      else
      {
        printf("Sent to queue");
        logging_function(getppid(),getpid(),"queues.log"," sending to message queue 1",src_ptr); 
      }
      if(mq_receive(ser_discriptor,(char *)dst_ptr,sizeof(msg_struct),0) < 0)
      {
        printf("Error receiving msg to  queue\n");
        status_function(getppid(),getpid(),"queues.log","Error to recieve message queue 1"); 
      }
      else
      {
        printf("received from queue");
        logging_function(getppid(),getpid(),"queues.log"," recieved message from queue 1",src_ptr); 
      }
    }
    mq_close(ser_discriptor);
  }
  else
  {
    struct mq_attr attribute;
    msg_struct *src_ptr = (msg_struct *)malloc(sizeof(msg_struct));
    msg_struct *dst_ptr = (msg_struct *)malloc(sizeof(msg_struct));
    attribute.mq_maxmsg = 10;
    attribute.mq_msgsize = sizeof(msg_struct);
    //attribute.mq_flags = 0;
    //attribute.mq_curmsgs = 0;

    cli_discriptor = mq_open ("/one_queue", O_RDWR | O_CREAT, 0666, &attribute);
    if (cli_discriptor < 0)
    {
      printf("message queue error\n");
      status_function(getppid(),getpid(),"queues.log","Error creating message queue"); 
    }
    else
    {
      printf("Successful in creating message queue\n");
      status_function(getppid(),getpid(),"queues.log","Successful in creating message queue"); 
    }
    for(int i = 0; i< 10; i++)
    {
      src_ptr->led_control ^= 1;
      sprintf(src_ptr->string, "From child to parent msg number:%d\n",i);
      src_ptr->string_length = strlen(src_ptr->string);
      sleep(1);
      if(mq_receive(cli_discriptor,(char *)dst_ptr,sizeof(msg_struct),0) < 0)
      {
        printf("Error receiving msg to  queue\n");
        status_function(getppid(),getpid(),"queues.log","Error to recieve message queue 2"); 
      }
      else
      {
        printf("received from queue");
        logging_function(getppid(),getpid(),"queues.log"," recieved message from queue 2",src_ptr); 
      }

      if(mq_send(cli_discriptor,(char *)src_ptr,sizeof(msg_struct),0) < 0)
      {
        printf("Error sending to  queue\n");
        status_function(getppid(),getpid(),"queues.log","Error sending to message queue 2"); 
      }
      else
      {
        printf("Sent to queue");
        logging_function(getppid(),getpid(),"queues.log"," sending to message queue 2",src_ptr); 
      }
    }
    mq_unlink("/one_queue");
    mq_close(cli_discriptor);
  }
  }

