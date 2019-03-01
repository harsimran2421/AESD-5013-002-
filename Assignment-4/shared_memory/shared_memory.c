#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include <sys/time.h>

#define MY_SEMAPHORE "/my_sem"
#define SHARED_MEMORY "/harry_space"

int shm_discriptor;

typedef struct{
    char string[50];
    int string_length;
    int led_control;
}msg_struct; 

void intHandler(int dummy) {
  FILE *file_ptr=fopen("shm.log","a");
  fprintf(file_ptr,"CTRL + C SIGNAL CAUGHT!!!!");
  fclose(file_ptr);
  shm_unlink(SHARED_MEMORY);
  close(shm_discriptor);
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
  //sleep(1);
  void *shared_mem_ptr;
  sem_t *my_semaphore;
  msg_struct *src_ptr = (msg_struct *)malloc(sizeof(msg_struct));
  msg_struct *dst_ptr = (msg_struct *)malloc(sizeof(msg_struct));
  shm_discriptor = shm_open(SHARED_MEMORY, O_RDWR | O_CREAT , 0666);
  if (shm_discriptor < 0)
  {
    printf("Error while opening shared memory");
    status_function(getppid(),getpid(),"shm.log","Error creating shared memory"); 
  }
  else
  {
    status_function(getppid(),getpid(),"shm.log","Successful in creating shared memory"); 
  }
  shared_mem_ptr = mmap(NULL, sizeof(msg_struct)+1, PROT_WRITE | PROT_READ, MAP_SHARED, shm_discriptor, 0);
  if (shared_mem_ptr == NULL)
  {
    printf("Error while mmap\n");
    status_function(getppid(),getpid(),"shm.log","Error in mmap"); 
  }
  else
  {
    status_function(getppid(),getpid(),"shm.log","Successful in MMAP"); 
  }
  if((my_semaphore = sem_open(MY_SEMAPHORE,O_CREAT,0666,0)) == NULL)
  {
    printf("Error in sem_open");
    status_function(getppid(),getpid(),"shm.log","Error in sem_open"); 
  }
  else
  {
    printf("successful in sem_open");
    status_function(getppid(),getpid(),"shm.log","successfull in sem_open"); 
  }
 volatile char *signal;
 signal = shared_mem_ptr + sizeof(msg_struct);
 for(int i = 0; i < 10 ;i++)
 {
  //sem_wait(my_semaphore);
  while(*signal == 'r');
  memcpy((char *)dst_ptr,(char *)shared_mem_ptr, sizeof(msg_struct));
  logging_function(getppid(),getpid(),"shm.log"," (child)recieved message from shared memory",dst_ptr); 
  src_ptr->led_control ^= 1;
  sprintf(src_ptr->string, "From shm child to shm parent msg number:%d\n",i);
  src_ptr->string_length = strlen(src_ptr->string);
  logging_function(getppid(),getpid(),"shm.log"," (child)sending message from shared memory",src_ptr);  
  memcpy((char *)shared_mem_ptr, (char *)src_ptr,sizeof(msg_struct));
  *signal = 'r';
  //sem_post(my_semaphore);
 }
  sem_unlink(SHARED_MEMORY);
  close(shm_discriptor);
}
else
{
  void *shared_mem_ptr;
  sem_t *my_semaphore;
  int truncate_value;
  shm_discriptor = shm_open(SHARED_MEMORY, O_RDWR | O_CREAT , 0666);
  msg_struct *src_ptr = (msg_struct *)malloc(sizeof(msg_struct));
  msg_struct *dst_ptr = (msg_struct *)malloc(sizeof(msg_struct));

    if (shm_discriptor < 0)
    {
      printf("Error while opening shared memory");
      status_function(getppid(),getpid(),"shm.log","Error creating shared memory"); 
    }
    else
    {
      status_function(getppid(),getpid(),"shm.log","Successful in creating shared memory"); 
    }
    if((truncate_value = ftruncate(shm_discriptor, sizeof(msg_struct)+1)) < 0)
    {
	    printf("Error while truncating memory");
      status_function(getppid(),getpid(),"shm.log","Error in truncating shared memory"); 
    }
    else
    {
      status_function(getppid(),getpid(),"shm.log","successful in truncating shared memory"); 
    }
    shared_mem_ptr = mmap(NULL, sizeof(msg_struct)+1, PROT_WRITE | PROT_READ, MAP_SHARED, shm_discriptor, 0);
    if (shared_mem_ptr == NULL)
    {
      printf("Error while mmap\n");
      status_function(getppid(),getpid(),"shm.log","Error in mmap"); 
    }
    else
    {
      status_function(getppid(),getpid(),"shm.log","Successful in MMAP"); 
    }
    if((my_semaphore = sem_open(MY_SEMAPHORE,O_CREAT,0666,0)) == NULL)
    {
      printf("Error in sem_open");
      status_function(getppid(),getpid(),"shm.log","Error in sem_open"); 
    }
    else
    {
      printf("successful in sem_open");
      status_function(getppid(),getpid(),"shm.log","successfull in sem_open"); 
    }
    volatile char *signal;
    signal = shared_mem_ptr + sizeof(msg_struct);
    for(int i = 0; i<10; i++)
    {
      sleep(1);
    src_ptr->led_control ^= 1;
    sprintf(src_ptr->string, "From shm parent to shm child msg number:%d\n",i);
    src_ptr->string_length = strlen(src_ptr->string);
    logging_function(getppid(),getpid(),"shm.log","(parent) sending message in shared memory",src_ptr); 
    memcpy((char *)shared_mem_ptr, (char *)src_ptr,sizeof(msg_struct)+1);
    *signal = 'w';
    printf("child recieved!!!!!\n");
    while(*signal == 'w');
    memcpy((char *)dst_ptr,(char *)shared_mem_ptr, sizeof(msg_struct));
    logging_function(getppid(),getpid(),"shm.log","(parent) recieved message in shared memory",dst_ptr); 
    }
  	  int shul = shm_unlink(SHARED_MEMORY);
}
}
