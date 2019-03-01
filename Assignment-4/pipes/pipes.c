#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>


typedef struct{
    char string[50];
    int led_control;
    int string_length;
}msg_struct; 

int flag;
int p2c_pipe[2], c2p_pipe[2];

void intHandler(int dummy) {
  FILE *file_ptr=fopen("pipes.log","a");
  fprintf(file_ptr,"CTRL + C SIGNAL CAUGHT!!!!");
  fclose(file_ptr);
  close(c2p_pipe[1]);
  close(c2p_pipe[0]);
  close(p2c_pipe[0]);
  close(p2c_pipe[1]);
  close(p2c_pipe[1]);
  close(p2c_pipe[0]);
  close(c2p_pipe[0]);
  close(c2p_pipe[1]);
  exit(0);
}


void logging_function(int parent_id, int pthread_id, char *file_name,char* ip_str)
{
  FILE *file_ptr = fopen(file_name,"a");
  struct timeval curr_time;
  gettimeofday(&curr_time, NULL);
  fprintf(file_ptr,"[Timestamp: %ld us] Parent ID:%d, Posix thread ID:%d",curr_time.tv_usec,parent_id, pthread_id);
  fprintf(file_ptr,"%s\n",ip_str);
  fflush(file_ptr);
  fclose(file_ptr);
}

int main(int argc, char *argv[])
{
  signal(SIGINT, intHandler);
  char *file_name = argv[1];
  /*variable to store return value of the fork function*/
  pid_t process_id;
  /*from parent to child, parent write, child read*/
  memset(p2c_pipe,0,sizeof(p2c_pipe));
  memset(c2p_pipe,0,sizeof(c2p_pipe));
  /*create pipes for bi directional communication*/
  if (pipe(p2c_pipe) < 0)
  {                            
    logging_function(getppid(),getpid(),file_name,"Error creating p2c_pipe");
    perror("Error in creating p2c_pipe");
    printf("p2c_pipe pipe error\n");
  }
  else
  {
    logging_function(getppid(),getpid(),file_name,"p2c_pipe created successfully");
    printf("p2c_pipe pipe success\n");
  }
  if (pipe(c2p_pipe) < 0)                            // create the pipe
  {                             
    perror("pipe error");
    printf("pipe_child2parent pipe error\n");
    logging_function(getppid(),getpid(),file_name,"Error creating c2p_pipe");
  }
  else
  {
    logging_function(getppid(),getpid(),file_name,"c2p_pipe created successfully");
    printf("p2c_pipe pipe success\n");
  }
  process_id = fork();
  if (process_id == 0)
  {
    char ip_str[200];
    memset(ip_str,0,sizeof(ip_str));
    flag = 0;
    // child process
    printf("Child process\n");
    msg_struct *ptr;
    msg_struct src_struct;
    ptr = &src_struct;

    // child process read
    char buff[sizeof(msg_struct)]={0};
    //memset(buff,0,sizeof(msg_struct));
    for(int i = 0; i< 10; i++)
    {
      memset(ip_str,0,sizeof(ip_str));
      read(p2c_pipe[0], buff, sizeof(msg_struct));
      ptr = (msg_struct*)(buff);
      sprintf(ip_str,"Child Received string: %s, String length = %d, led on: %d\n", ptr->string, ptr->string_length, ptr->led_control);
      logging_function(getppid(),getpid(),file_name,ip_str); 

      // child process write        
      sprintf(src_struct.string, "From child to parent msg %d",i);
      src_struct.string_length = strlen(src_struct.string);
      src_struct.led_control ^= 1;
      ptr = &src_struct;
      write(c2p_pipe[1], ptr, sizeof(msg_struct));
      //logging_function(getppid(),getpid(),file_name,); 
      printf("Data sent from child to parent\n");
    }
    close(c2p_pipe[1]);
    close(c2p_pipe[0]);
    close(p2c_pipe[0]);
    close(p2c_pipe[1]);
    exit(1);
  }
  else
  {
    char ip_str[200];
    flag = 1;
    // parent process
    printf("Parent process\n");
    // parent process write
    msg_struct *ptr;
    msg_struct src_struct;
    ptr = &src_struct;
    for(int i = 0; i<10;i++)
    {
      sleep(1);
      memset(ip_str,0,sizeof(ip_str));
      //memset(ptr,0,sizeof(mesg_struct));
      sprintf(src_struct.string, "From parent to child msg %d",i);
      src_struct.string_length = strlen(src_struct.string);
      src_struct.led_control = 1;
      write(p2c_pipe[1], ptr, sizeof(msg_struct));
      printf("Data sent from parent to child\n");
      while(!flag);
      // parent process read
      char buff[sizeof(msg_struct)];
      //memset(buff,0,sizeof(msg_struct));
      int ret = read(c2p_pipe[0], buff, sizeof(msg_struct));
      ptr = (msg_struct*)(buff);
      sprintf(ip_str,"Parent Received string: %s, String length = %d, led on: %d\n", ptr->string, ptr->string_length, ptr->led_control);
      logging_function(getppid(),getpid(),file_name,ip_str); 
    }
    close(c2p_pipe[0]);
    close(p2c_pipe[1]);
    close(c2p_pipe[1]);
    close(p2c_pipe[0]); //close read pipe
  }    
}
