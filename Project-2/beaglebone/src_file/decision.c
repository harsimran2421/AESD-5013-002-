#include "uart.h"
#include "decision.h"
#include "logger.h"

void *decision(void *arg)
{
  thread_struct *thread_input = (thread_struct *)arg; 
  int file_descriptor = 0;
  int decision_recieve;
  int ret_val = uart_init(PORT1,&file_descriptor);
  struct termios terminal_var;
  printf("ret_val for uart_init = %d", ret_val);

  ret_val = termios_init(file_descriptor,&terminal_var);
  printf("ret_val for terminos_init = %d", ret_val);
  //    sleep(1);
  struct mq_attr dec_attribute1;
  dec_attribute1.mq_maxmsg = 10;
  dec_attribute1.mq_maxmsg = sizeof(msg_struct);
  decision_recieve = mq_open("/decision_queue",O_RDWR | O_CREAT, 0666, &dec_attribute1);
  msg_struct *temp_msg = (msg_struct *)malloc(sizeof(msg_struct));
  temp_msg->thread_id = 0;
  temp_msg->sensor_value = 0;
if(decision_recieve < 0)
{
  printf("Decision Recv Queue Error!");
  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Recv Creation Failed!",temp_msg);
}
else 
{
  printf("Decision Recv Queue Successfully craeted");
  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Recv Creation Successfull!",temp_msg);
}
msg_struct input_msg;
msg_struct send_value;
while(exit_flag != 1)
{
  sleep(1);  
  read_from_uart(&file_descriptor,&input_msg);
  if(input_msg.thread_id == ALCOHOL)
  {
    if(input_msg.sensor_value > ALCOHOL_THRESHOLD)
    {
      send_value.sensor_value = input_msg.sensor_value;
      send_value.thread_id = 2;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision Alcohol\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision Alcohol",temp_msg);
      }
    }
    else       
    {
      send_value.sensor_value = input_msg.sensor_value;
      send_value.thread_id = 2;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision Alcohol\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision Alcohol",temp_msg);
      }
    }
	decision_flag = 1;
  }
  if(input_msg.thread_id == ULTRASONIC)
  {
    if(input_msg.sensor_value < ULTRASONIC_THRESHOLD)
    {
      send_value.sensor_value = input_msg.sensor_value;
      send_value.thread_id = 3;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision ultrasonic\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision ultrasonic",temp_msg);
      }
    }
    else       
    {
      send_value.sensor_value = input_msg.sensor_value;
      send_value.thread_id = 3;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision ultrasonic\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision ultrasonic",temp_msg);
      }
    }
	decision_flag = 1;
  }
  if(input_msg.thread_id == 4)
  {
	  send_value.sensor_value = input_msg.sensor_value;
	  send_value.thread_id = 2;
	  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Alcohol sensor disconnected",temp_msg);
  }
  if(input_msg.thread_id == 5)
  {
	  send_value.sensor_value = input_msg.sensor_value;
	  send_value.thread_id = 3;
	  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Ultrasonic sensor disconnected",temp_msg);
  }


}
//usleep(15000);
logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Thread Exit!",temp_msg);
pthread_exit(NULL);
}


