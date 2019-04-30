/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : uart.h
* Creation Date :28-04-2019
* Last Modified : Sun 28 April 2019 20:10:11 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: uart file for uart function declaration and structure declarations
* 
* Functions:
*	    uart_status uart_init() is a uart initailization function
*	    int termios_init() is a terminal setup function
*           uart_status write_to_uart() is a function to write to uart
*           uart_status read_from_uart() is a function to read from uart
* References:
_._._._._._._._._._._._._._._._._._._._._.*/

#ifndef MY_UART_H_
#define MY_UART_H_

/*Standard library*/
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

/*User library*/
#include "user_led.h"
#include "uart.h"

/*Macros*/
#define PORT1_PATH	"/dev/ttyO1"
#define PORT2_PATH	"/dev/ttyO2"
#define PORT4_PATH	"/dev/ttyO4"

/*thread structure*/
typedef struct
{
	uint8_t thread_id;
	float sensor_value;
}msg_struct;

/*Port enumeration */
typedef enum
{
  PORT1 = 1,
  PORT2 = 2,
  PORT4 = 4
}port_number;

/*error enumeration */
typedef enum
{
	SEND_ERROR=-10,
	RECEIVE_ERROR=-20,
	INITIALIZATION_ERROR =-30,
	SUCCESS
}uart_status;

/* -------------------------------*/
/**
 * @Synopsis is a uart initailization function
 *
 * @Param port_no is a port number varaible of port_number data type
 * @Param file descriptor is a variableto define the file path
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
uart_status uart_init(port_number port_no, int *file_descriptor);
/* -------------------------------*/
/**
 * @Synopsis is a terminal setup function 
 * @Param file descriptor is a variable to define the file path
 * @Param termi is a variable of termios data type
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
int termios_init(int file_descriptor, struct termios *term);

/* -------------------------------*/
/**
 * @Synopsis is a function to write to uart 
 *
 * @Param filedes is a variable file decriptor 
 * @Param value is a value to be written to uart
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
uart_status write_to_uart(int filedes, uint8_t value);
/* -------------------------------*/
/**
 * @Synopsis is a function to read from uart 
 * @Param filedes is a variable file decriptor 
 * @Param rx_data is a value to store the read value
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
uart_status read_from_uart(int *filedes, msg_struct *rx_data);
#endif
