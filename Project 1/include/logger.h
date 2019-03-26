#ifndef _LOGGER_H
#define _LOGGER_H

/*standard headers*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <errno.h>

/*system headers*/
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/stat.h>

/*POSIX thread*/
#include <pthread.h>  

/*IPC headers*/
#include <netinet/in.h> 
#include <mqueue.h>
#include <netdb.h>

/*I2C headers*/
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

pthread_mutex_t bus_lock;

typedef struct thread_content{

    pthread_t tid;         // the thread identifier
    char* log_file;

}thread_struct;

typedef struct message_struct
{
  char thread_name[20];
  char level[6];
  float sensor_value;
  char unit;

}msg_struct;

int light_flag;
int temperature_flag;



void *logging_thread(void *arg);
void logging_function(int parent_id, int pthread_id, int thread_id, char *file_name,char* ip_str,msg_struct *msg);
#endif
