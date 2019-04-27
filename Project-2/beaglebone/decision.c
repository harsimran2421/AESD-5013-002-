typedef struct decision_struct_send
{
  int thread_name;
 
  int decision;
} dec_struct_send;

typedef struct decision_struct_recv
{
  char thread_id[10];
  float sensor_value;
} dec_struct_recv;

void *decision()
{  
    struct mq_attr dec_attribute1;
    dec_attribute1.mq_maxmsg = 10;
    dec_attribute1.mq_maxmsg = sizeof(dec_struct);
    decision_recieve = mq_open("/Decision_queue",O_RDWR | O_CREAT, 0666, &dec_attribute1);
    if(decision_recieve < 0)
    {
      printf("Decision Recv Queue Error!");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Recv Creation Failed!");
    }
    else 
    {
      printf("Decision Recv Queue Successfully craeted");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Recv Creation Successfull!");
}
    struct mq_attr dec_attribute2;
    dec_attribute2.mq_maxmsg = 10;
    dec_attribute2.mq_maxmsg = sizeof(dec_struct);
    decision_send = mq_open("/Decision_queue",O_RDWR | O_CREAT, 0666, &dec_attribute2);
    if(decision_send < 0)
    {
      printf("Decision Send Queue Error!");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Send Creation Failed!");
    }
    else 
    {
      printf("Decision Send Queue Successfully craeted");
      logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Creation Successfull!");
    }
    while(exit_flag != 1)
    {
      dec_struct_recv *input_msg = (dec_struct *)malloc(sizeof(dec_struct));
      if(mq_receive(decision_recieve,(char*)input_msg,sizeof(dec_struct_recv),NULL) < 0);
      {
        printf("Error recieving from UART");
        logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error Recieving from UART!");
      }
      if(input_msg->thread_id == alcohol)
      {
          if(input_msg->sensor_value > ALCOHOL_THRESHOLD)
          {
            dec_struct *msgA = (dec_struct *)malloc(sizeof(dec_struct));
            memeset(msgA->thread_name,0,sizeof(msgA->thread_name));
            memcpy(msgA->thread_name,1,strlen("decision High"));
            memset(msgA->decision,0,sizeof(msgA->decision));
            memcpy(msgA->decision,10,sizeof(msgA->decision));
            if(mq_send(decision_send,(const char *)&msgA, sizeof(msg_struct),0) == -1)
            {
              printf("\nERROR: Sending Decision Alcohol\n");
              logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision Alcohol");
              mq_close(decision_send);
            }
            logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Alcohol Send: ALERT: %s %s",msgA->thread_name, msgA->decision);
          }
          else       
          {
            dec_struct *msgA = (dec_struct *)malloc(sizeof(dec_struct));
            memeset(msgA->thread_name,0,sizeof(msgA->thread_name));
            memcpy(msgA->thread_name,1,sizeof(int));
            memset(msgA->decision,0,sizeof(msgA->decision));
            memcpy(msgA->decision,9,sizeof(msgA->decision));
            if(mq_send(decision_send,(const char *)&msgA, sizeof(msg_struct),0) == -1)
            {
              printf("\nERROR: Sending Decision Alcohol\n");
              logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Error! Sending Decision Alcohol");
              mq_close(decision_send);
            }
            logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Alcohol Send: NORMAL: %s %s",msgA->thread_name, msgA->decision);

          }
      }
      else if(input->msg->thread_id == ultrasonic_task)
      {
          if(input->sensor_value > ULTRASONIC_THRESHOLD)
          {
            dec_struct_send *msgB = (msg_struct *)malloc(sizeof(msg_struct));
            memeset(msgB->thread_name,0,sizeof(msgA->thread_name));
            memcpy(msgB->thread_name,2,sizeof(int));
            memset(msgA->decision,0,sizeof(msgA->decision));
            memcpy(msgA->decision,20,sizeof(msgA->decision));
            
            if(mq_send(decision_send,(const char *)&msgB, sizeof(msg_struct),0) == -1)
            {
              printf("\nERROR: Sending Decision Ultrasonic\n");
              logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Sending Failed!");
              mq_close(decision_send);
            }
            logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Ultrasonic Send: NORMAL: %s %s",msgA->thread_name,msgA->decision);

          }
          else
          {
            dec_struct_send *msgB = (msg_struct *)malloc(sizeof(msg_struct));
            memeset(msgB->thread_name,0,sizeof(int));
            memcpy(msgB->thread_name,2,sizeof(int));
            memset(msgA->decision,0,sizeof(msgA->decision));
            memcpy(msgA->decision,99,sizeof(msgA->decision));
            
            if(mq_send(decision_send,(const char *)&msgB, sizeof(msg_struct),0) == -1)
            {
              printf("\nERROR: Sending Decision Ultrasonic\n");
              logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Queue Sending Failed!");
              mq_close(decision_send);
            }
            logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Ultrasonic send: ALERT: %s %s",msgA->thread_name,msgA->decision);
          }
        }
    }
  usleep(15000);
  logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Decision Thread Exit!");
  pthread_exit(NULL);
}
