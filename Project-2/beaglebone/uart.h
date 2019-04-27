#ifndef MY_UART_H_
#define MY_UART_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define PORT1_PATH	"/dev/ttyO1"
#define PORT2_PATH	"/dev/ttyO2"
#define PORT4_PATH	"/dev/ttyO4"

typedef struct
{
	uint8_t thread_id;
	uint32_t timestamp;
	char ip_str[20];
	double sensor_value;
}msg_struct_uart;

typedef enum
{
  PORT1 = 1,
  PORT2 = 2,
  PORT4 = 4
}port_number;

typedef enum
{
	SEND_ERROR=-10,
	RECEIVE_ERROR=-20,
	INITIALIZATION_ERROR =-30,
	SUCCESS
}uart_status;

uart_status uart_init(port_number port_no, int *file_descriptor);
int termios_init(int file_descriptor, struct termios *term);

uart_status read_from_uart(int filedes);
#endif
