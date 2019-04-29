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
#include "user_led.h"
#include "uart.h"

#define PORT1_PATH	"/dev/ttyO1"
#define PORT2_PATH	"/dev/ttyO2"
#define PORT4_PATH	"/dev/ttyO4"

typedef struct
{
	uint8_t thread_id;
	float sensor_value;
}msg_struct;

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

//loopback testing
uart_status write_to_uart(int filedes, uint8_t value);
uart_status read_from_uart(int *filedes, msg_struct *rx_data);
#endif
