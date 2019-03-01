/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : main.c
* Creation Date : 21-02-2019
* Last Modified : Thu 28 Feb 2019 17:08:06 PM MDT
* Created By : Harsimransingh
* Description: Source file to implement 2 threads - 1 for character read and 1 for cpu utilization logging. 
* Refrences:
*           https://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
*           https://www.geeksforgeeks.org/multithreading-c-2/
_._._._._._._._._._._._._._._._._._._._._.*/



/*standard libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*pthread library*/
#include <pthread.h>

/*time function library*/
#include <time.h>

/*system libraries*/
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/syscall.h>

typedef struct thread_content{

    pthread_t tid;         // the thread identifier

    char* filename;
    char* log_file;

}thread_struct;

timer_t timer_id;
pthread_t cpu_utilization, character_read;
volatile int exit_flag = 0;
void lower_case(char* str)
{
    if(*str >= 'A' && *str <= 'Z')
    {
      *str = *str + 32;
    }
}

void logging_function(int parent_id, int pthread_id, int thread_id, char *file_name,char* ip_str)
{
  FILE *file_ptr = fopen(file_name,"a");
  struct timeval curr_time;
  gettimeofday(&curr_time, NULL);
  fprintf(file_ptr,"[Timestamp: %ld] Parent ID:%d, Posix thread ID:%d, Linux Thread ID:%d, ",(curr_time.tv_usec/1000),parent_id, pthread_id, thread_id);
  fprintf(file_ptr,"%s\n",ip_str);
  fflush(file_ptr);
  fclose(file_ptr);
}

void* read_function(void *arg)
{
  int alphabets[26];
  memset(alphabets,0,sizeof(alphabets));
  thread_struct* read_thread = (thread_struct *)arg;
  FILE *file_ptr;
  logging_function(getppid(),getpid(),syscall(SYS_gettid),read_thread->log_file,"read thread executing");
  file_ptr = fopen(read_thread->filename, "r");
  if(file_ptr == NULL)
  {
    printf("File open error");
  }
  while (!feof(file_ptr))
  {
    char ip_character = fgetc(file_ptr);
    lower_case(&ip_character);
    if(ip_character >='a' && ip_character <='z')
    {
      alphabets[ip_character - 97] += 1;
    }
  }
  fclose(file_ptr);
  logging_function(getppid(),getpid(),syscall(SYS_gettid),read_thread->log_file,"read thread exiting");
  file_ptr=fopen(read_thread->log_file, "a");
  for(int i = 0; i<26; i++)
  {
    if(alphabets[i] <=100 && alphabets[i]>0)
    {
      fprintf(file_ptr,"Alphabet :%c - count:%d\n",(i + 'a'),alphabets[i]);
    }
  }
  fclose(file_ptr);
  pthread_exit(NULL);
}
void sig_handler(union sigval sv)
{
  char *file_name = sv.sival_ptr;
  FILE *file_ptr;
  char *cpu_data = "cat /proc/stat | head -1 > command.txt";
  system(cpu_data);
  FILE * cpu_ptr = fopen("command.txt", "r");
  logging_function(getppid(),getpid(),syscall(SYS_gettid),file_name,"Timer event occured CPU utilization is:");
  if(cpu_ptr)
  {
    file_ptr = fopen(file_name,"a");
    char tmp[100];
    memset(tmp,0,sizeof(tmp));
    fread(tmp,1,100,cpu_ptr);
    fprintf(file_ptr,"%s",tmp);
    fprintf(file_ptr,"\n");
    fclose(file_ptr);
    fclose(cpu_ptr);
  } 
}

void usr_sig_handler(int sig)
{
		printf("SIGUSR1 or SIGUSR2 received, exiting the thread.\n");
    timer_delete(timer_id);
    printf("\n\nTIMER DELETED SUCCESFuLLY\n\n");
    exit_flag = sig;

}

void* cpu_function(void *arg)
{
  thread_struct *cpu_thread = (thread_struct *)arg;
  logging_function(getppid(),getpid(),syscall(SYS_gettid),cpu_thread->log_file,"cpu thread executing");
  struct sigevent sev;
  memset(&sev,0,sizeof(struct sigevent));
  struct itimerspec trigger;
  memset(&trigger,0,sizeof(struct itimerspec));
  struct sigaction user_signal;

  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = &sig_handler;
  sev.sigev_value.sival_ptr = cpu_thread->log_file;
  timer_create(CLOCK_REALTIME, &sev, &timer_id);

  memset(&user_signal, 0, sizeof(user_signal));
  user_signal.sa_handler = &usr_sig_handler;
  sigaction(SIGUSR1, &user_signal, NULL);
  sigaction(SIGUSR2, &user_signal, NULL);

  trigger.it_value.tv_sec=0;
  trigger.it_value.tv_nsec=100000000;
  trigger.it_interval.tv_sec=0;
  trigger.it_interval.tv_nsec=100000000;

  timer_settime(timer_id,0, &trigger, NULL);
  while(1)
  {
    if(exit_flag)
    {
      if(exit_flag == SIGUSR1)
      {
        logging_function(getppid(),getpid(),syscall(SYS_gettid),cpu_thread->log_file,"cpu thread exiting due to SIGUSR1 signal");
      }
      else if(exit_flag ==  SIGUSR2)
      {
        logging_function(getppid(),getpid(),syscall(SYS_gettid),cpu_thread->log_file,"cpu thread exiting due to SIGUSR2 signal");
      }
      else
      {
        logging_function(getppid(),getpid(),syscall(SYS_gettid),cpu_thread->log_file,"cpu thread exiting due to unknown signal");
      }
      printf("CPU Thread exit successful with signal: %d\n",exit_flag);
      pthread_exit(NULL);
    }
  }
  return NULL;
}

void main(int argc, char * argv[])
{
  /*two threads for monitoring cpu utilization and one to read characters from a file*/
  thread_struct *thread_input = (thread_struct *)malloc(sizeof(struct thread_content));
  thread_input->filename = "gdb.txt";
  thread_input->log_file = argv[1];
  if(!pthread_create(&character_read, NULL, read_function, (void *)thread_input))
  {
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Character read Thread created Successfully");
  }
  else
  {
    printf("Character read Thread creation failed\n");
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"Character read Thread creation failed");
  }
  pthread_join(character_read, NULL);
  if(!pthread_create (&cpu_utilization, NULL, cpu_function, (void*)thread_input))
  {
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"cpu thread created successfully");
  }
  else
  {
    printf("CPU Utilization Thread creation failed\n");
    logging_function(getppid(),getpid(),syscall(SYS_gettid),thread_input->log_file,"CPU Utilization Thread creation failed");
  }
  pthread_join(cpu_utilization, NULL);
  printf("\nREACHED MAIN\n");
}

