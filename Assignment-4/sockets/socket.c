#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int sock_client;                               //this will be our socket
int sock;
typedef struct{
    char string[50];
    int string_length;
    int led_control;
}msg_struct; 


void intHandler(int dummy) {
  FILE *file_ptr=fopen("socket.log","a");
  fprintf(file_ptr,"CTRL + C SIGNAL CAUGHT!!!!");
  fclose(file_ptr);
  close(sock);
  close(sock_client);
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


#define MAXBUFSIZE 200
#define SEND_BUFFER_SIZE 512
#define RECIEVE_BUFFER_SIZE 512

int main (int argc, char * argv[] )
{
  signal(SIGINT, intHandler);
  pid_t process_id;
  process_id = fork();
  if(process_id == 0)
  {
    msg_struct *ptr;
    msg_struct src_struct;
    ptr = &src_struct;
    msg_struct *dst_ptr;
    msg_struct dst_struct;
    dst_ptr = &dst_struct;
 
    printf("code started\n");
    /*This will be our socket*/
    int sock;                           
    /*Internet socket address structure*/
    struct sockaddr_in sin, remote;
    /*length of the sockaddr_in structure*/
    unsigned int remote_length;          
    int nbytes,nbytes_send,nbytes_recieve, return_value;                        //number of bytes we receive in our message

    /*File pointer to read and write files*/
    FILE *file_pointer;             
    /*Local variables for file transfer*/
    int file_size, loop_count, last_batch;
    /*buffer to load and send data bytes*/
    char send_buffer[SEND_BUFFER_SIZE];
    /*buffer to recieve and write data bytes*/
    char recieve_buffer[RECIEVE_BUFFER_SIZE]; 

    bzero(&sin,sizeof(sin));                    //zero the struct
    sin.sin_family = AF_INET;                   //address family
    sin.sin_port = htons(atoi(argv[1]));        //htons() sets the port # to network byte order
    sin.sin_addr.s_addr = INADDR_ANY;           //supplies the IP address of the local machine


    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      printf("unable to create socket");
    }
    else
      printf("socket created\n");
    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
      printf("unable to bind socket\n");
    }
    else
      printf("binding successful\n");
    remote_length = sizeof(remote);
      bzero(recieve_buffer,RECIEVE_BUFFER_SIZE);
for(int i = 0; i < 10; i++)
{
      nbytes_recieve = recvfrom(sock,dst_ptr,sizeof(msg_struct), 0, (struct sockaddr *)&remote, &remote_length);
      if(nbytes_recieve > 0)
      {
        printf("recieved string is: %s\n",recieve_buffer);
      }
      logging_function(getppid(),getpid(),"socket.log","server recieved string",dst_ptr); 
      sprintf(src_struct.string, "From server to client msg %d",i);
      src_struct.string_length = strlen(src_struct.string);
      src_struct.led_control ^= 1;
      ptr = &src_struct;
      printf("Data sent from server to client\n");
      logging_function(getppid(),getpid(),"socket.log","server sending string",ptr); 
      nbytes_send = sendto(sock, ptr,sizeof(msg_struct), 0, (const struct sockaddr *)&remote, remote_length);
}
      close(sock);
  }
  else
  {
    msg_struct *ptr;
    msg_struct src_struct;
    ptr = &src_struct;
    msg_struct *dst_ptr;
    msg_struct dst_struct;
    dst_ptr = &dst_struct;
    sleep(1);
    int nbytes, nbytes_send, nbytes_recieve;                             // number of bytes send by sendto()
    /*buffer to recieve and write data bytes*/
    char recieve_buffer[RECIEVE_BUFFER_SIZE];
    /*buffer to load and send data bytes*/
    char send_buffer[SEND_BUFFER_SIZE];

    /*Local variables for file transfer*/
    int loop_count = 0,return_value;
    struct sockaddr_in remote;              //"Internet socket address structure"
    struct sockaddr_in from_addr;
    int addr_length = sizeof(struct sockaddr);
    FILE *file_pointer;
    if (argc < 3)
    {
      printf("USAGE:  <server_ip> <server_port>\n");
      exit(1);
    }

    bzero(&remote,sizeof(remote));               //zero the struct
    remote.sin_family = AF_INET;                 //address family
    remote.sin_port = htons(atoi(argv[1]));      //sets port to network byte order
    remote.sin_addr.s_addr = inet_addr(argv[2]); //sets remote IP address

    //Causes the system to create a generic socket of type UDP (datagram)
    if ((sock_client = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      printf("unable to create socket");
    }
    printf("Socket created\n");
    for(int i = 0; i< 10; i++)
    {
      sleep(1);
      sprintf(src_struct.string, "From Client to server msg %d",i);
      src_struct.string_length = strlen(src_struct.string);
      src_struct.led_control ^= 1;
      ptr = &src_struct;
      printf("Data sent from child to parent\n");
      logging_function(getppid(),getpid(),"socket.log","Client sending string",ptr); 
      nbytes_send = sendto(sock_client, ptr, sizeof(msg_struct), 0, (const struct sockaddr *)&remote, sizeof(remote)); 
      nbytes_recieve = recvfrom(sock_client,dst_ptr, sizeof(msg_struct), 0, (struct sockaddr *)&from_addr, &addr_length);
      logging_function(getppid(),getpid(),"socket.log","Client recieved string",dst_ptr); 
      sleep(1);
    }
    close(sock_client);
  }
}
