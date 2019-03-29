#ifndef __SOCKET_H
#define __SOCKET_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>
#include<signal.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#define MAXBUFSIZE 200
#define SEND_BUFFER_SIZE 512
#define RECIEVE_BUFFER_SIZE 512
#define PORT_NUMBER 9000


void *socket_function(void *arg);

#endif
