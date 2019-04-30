/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : decision.c
* Creation Date :28-04-2019
* Last Modified : Sun 28 April 2019 20:10:11 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: decision file for decision function declaration and structure declarations
* 
* Functions:
*           decision() function to set decisio flags based on incoming sensor values
* References:
_._._._._._._._._._._._._._._._._._._._._.*/

/*user libraries*/
#include "uart.h"
#include "decision.h"
#include "logger.h"
#include "user_led.h"

/* -------------------------------*/
/**
 * @Synopsis decision task to set the decision flags for different sensors and communciate back the 
 * flag values to the Remote node using UART
 *
 * @Param arg to pass arguement to pthread 
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
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
  struct mq_attr dec_attribute1;
  dec_attribute1.mq_maxmsg = 10;
  dec_attribute1.mq_maxmsg = sizeof(msg_struct);
  decision_recieve = mq_open("/decision_queue",O_RDWR | O_CREAT, 0666, &dec_attribute1);
  msg_struct *temp_msg = (msg_struct *)malloc(sizeof(msg_struct));
  temp_msg->thread_id = 0;
  temp_msg->sensor_value = 0; 
  setup_led();
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
    led_control(GREEN,OFF);
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
      send_value.thread_id = 8;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision Alcohol\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision Alcohol",temp_msg);
      }
    }
	decision_flag = 1;
  }
  else if(input_msg.thread_id == ULTRASONIC)
  {
    led_control(BLUE,OFF);
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
      send_value.thread_id = 9;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision ultrasonic\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision ultrasonic",temp_msg);
      }
    }
	decision_flag = 1;
  }
  else if(input_msg.thread_id == TEMPERATURE)
  {
    led_control(BLUE,OFF);
    if(input_msg.sensor_value > TEMPERATURE_THRESHOLD)
    {
      send_value.sensor_value = input_msg.sensor_value;
      send_value.thread_id = 11;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision ultrasonic\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision temperature",temp_msg);
      }
    }
    else       
    {
      send_value.sensor_value = input_msg.sensor_value;
      send_value.thread_id = 10;
      if(mq_send(decision_recieve,(const char *)&send_value, sizeof(msg_struct),0) == -1)
      {
        printf("\nERROR: Sending Decision ultrasonic\n");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision temperature",temp_msg);
      }
    }
	decision_flag = 1;
  }

  else if(input_msg.thread_id == 4)
  {
    led_control(GREEN,ON);
	  send_value.sensor_value = input_msg.sensor_value;
	  send_value.thread_id = 2;

	  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Alcohol sensor disconnected",temp_msg);
  }
  else if(input_msg.thread_id == 5)
  {
    led_control(BLUE,ON);
	  send_value.sensor_value = input_msg.sensor_value;
	  send_value.thread_id = 3;
	  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Ultrasonic sensor disconnected",temp_msg);
  }
  else
  { 
	  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"UART connection disconnected",temp_msg);
  }
}
logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Thread Exit!",temp_msg);
pthread_exit(NULL);
}


