/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : logger.c
* Creation Date : 19-03-2019
* Last Modified : Sun 31 March 2019 00:10:11 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file for logger function declaration and structure declarations
* 
* Functions:
*           logging_thread() linked to logger pthread task
*           logging_function() to log messages in a common log file
*           timer_setup() to set timer attributes for heartbeat
* References:
_._._._._._._._._._._._._._._._._._._._._.*/

/*standard headers*/
#include "logger.h"

mqd_t decision_discriptor; 

/*strcuture to timestamp values*/
struct timespec timer_setup(uint32_t second_value, uint32_t nanosec_value)
{
  struct timespec ts;
  struct timeval tp;

  gettimeofday(&tp,NULL);
  ts.tv_sec = tp.tv_sec;
  ts.tv_nsec = tp.tv_usec * 1000;
  ts.tv_sec += second_value;
  ts.tv_nsec += nanosec_value;
  
  return ts;
}

/* -------------------------------*/
/**
 * @Synopsis linked to logger pthread task used for synchronized logging
 *
 * @Param arg to pass arguement to pthread 
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
void *logging_thread(void *arg)
{
  thread_struct *logging_thread = (thread_struct *)arg;
  /*light queue attribute settings*/
  mqd_t light_discriptor;
  struct mq_attr attribute;
  attribute.mq_maxmsg = 10;
  attribute.mq_msgsize = sizeof(msg_struct);
  decision_discriptor = mq_open ("/decision_queue", O_RDWR | O_CREAT, 0666, &attribute);
  if (light_discriptor < 0)
  {
    printf("message queue error\n");
  }
  else
  {
    printf("Successful in creating message queue\n");
  }

  while(exit_flag!=1)
  {
    pthread_cond_broadcast(&logger_thread_cond);
    if(decision_flag != 0)
    {
      decision_flag = 0;
      msg_struct *msg = (msg_struct *)malloc(sizeof(msg_struct));
      if(mq_receive(decision_discriptor,(char *)msg,sizeof(msg_struct),0) < 0)
      {
        printf("Error receiving msg to  temperature queue\n");
      }
      logging_function(getppid(),getpid(),syscall(SYS_gettid),logging_thread->log_file,"sensor_value recieved",msg);
    }
  }
  pthread_exit(NULL);
}

void logging_function(int parent_id, int pthread_id, int thread_id, char *file_name,char* ip_str,msg_struct *msg)
{
  pthread_mutex_lock(&logger_mutex); 
  FILE *file_ptr = fopen(file_name,"a");
  time_t T= time(NULL);
  struct  tm tm = *localtime(&T);
  fprintf(file_ptr,"\nSystem Date is: %02d/%02d/%04d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
  fprintf(file_ptr,"System Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
  if(msg != NULL)
  {
    if(msg->thread_id == 3)
    {
      	fprintf(file_ptr,"Thread:%s\nLog level:%s\nsensor_value:%.2f\n","Ultrasonic","DATA",msg->sensor_value);
    }
    else if(msg->thread_id == 9)
    {
    	fprintf(file_ptr,"Thread:%s\nLog level:%s\nsensor_value:%.2f\n","Ultrasonic THRESHOLD CROSSED","DATA",msg->sensor_value);
    }
    else if(msg->thread_id == 8)
    {
    	  fprintf(file_ptr,"Thread:%s\nLog level:%s\nsensor_value:%.2f\n","Alcohol","DATA",msg->sensor_value);
    }
    else if(msg->thread_id == 2)
    {
      	 fprintf(file_ptr,"Thread:%s\nLog level:%s\nsensor_value:%.2f\n","Alcohol THRESHOLD","DATA",msg->sensor_value);
    }
  }
  fprintf(file_ptr,"%s\n",ip_str);
  fflush(file_ptr);
  fclose(file_ptr);
  pthread_mutex_unlock(&logger_mutex); 
}


