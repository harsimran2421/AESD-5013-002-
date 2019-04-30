/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : uart.c
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

/*userdefined library*/
#include "uart.h"

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
uart_status uart_init(port_number port_no,int *file_descriptor)
{
	char *uart_path = NULL;
	if(port_no = PORT1)
	{
		uart_path = PORT1_PATH;
	}
	else if(port_no = PORT2)
	{
		uart_path = PORT2_PATH;
	}
	else if(port_no = PORT4)
	{
		uart_path = PORT4_PATH;
	}
	else
	{
		return INITIALIZATION_ERROR;
	}

	*file_descriptor = open(uart_path, O_RDWR | O_NOCTTY | O_SYNC);//O_NDELAY);

	if (*file_descriptor < 0)
	{
		perror("failed to open port, uart init failed! \n");
		return	INITIALIZATION_ERROR;
	}
	fcntl(*file_descriptor,F_SETFL,0);
	return SUCCESS;
}

/* -------------------------------*/
/**
 * @Synopsis is a terminal setup function 
 * @Param file descriptor is a variable to define the file path
 * @Param termi is a variable of termios data type
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
int termios_init(int file_descriptor, struct termios *term)
{
	tcgetattr(file_descriptor, term);

  term->c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON); 

  term->c_oflag = 0;

  term->c_lflag &= ~(ECHO | ECHONL | ICANON |/* ECHOK | ECHOE |*/ ISIG | IEXTEN);	

	term->c_cc[VMIN] = 1;

	term->c_cc[VTIME] = 0;

	tcflush(file_descriptor, TCIFLUSH);

  if( cfsetispeed(term,B115200) || cfsetospeed(term,B115200))
  {
    perror("ERROR set speed");
    return -1;
  }

	tcsetattr(file_descriptor, TCSAFLUSH, term);

	return 0;
}

/* -------------------------------*/
/**
 * @Synopsis is a function to write to uart 
 *
 * @Param filedes is a variable file decriptor 
 * @Param value is a value to be written to uart
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
uart_status write_to_uart(int filedes,uint8_t value) 
{

	int ret_val;
	if (ret_val = write(filedes, &value, 1) <1)
	{
		perror("Error writing data\n");
		return SEND_ERROR;
	}
	else
	{
		printf("data sent success\n");
		printf("ret_val write = %d\n", ret_val);
		return SUCCESS;
	}
	return 5;
}

/* -------------------------------*/
/**
 * @Synopsis is a function to read from uart 
 * @Param filedes is a variable file decriptor 
 * @Param rx_data is a value to store the read value
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
uart_status read_from_uart(int *filedes,msg_struct *rx_data)
{
	static int ultra_on_flag, alco_on_flag;
	int ret_val;
	int filedes1;
	static uint8_t ultra, alco;
	if(ret_val = read(*filedes, rx_data, sizeof(msg_struct)) <1)
	{
		perror("Error Reading data\n");
		read_from_uart(filedes,rx_data);
		return RECEIVE_ERROR;
	}
	else
	{
		if(rx_data->thread_id == 2)
		{	
			alco_on_flag = 1;
			led_control(RED,OFF);
			printf("\nalcohol task \t sensor value = %f\n", rx_data->sensor_value);
      			if(rx_data->sensor_value > 10)
        			alco = 6;
      			else
        			alco = 5;
			write_to_uart(*filedes,alco);
		}		
		else if(rx_data->thread_id == 3)
		{
			ultra_on_flag = 1;
			if(ultra_on_flag == 1 && alco_on_flag == 1)
			{
				printnumber(1);
			}
			led_control(RED,OFF);
      			if(rx_data->sensor_value < 10)
        			ultra = 1;
      			else
        			ultra = 0;
			printf("\nultrasonic task \t sensor value = %f\n", rx_data->sensor_value);	
			write_to_uart(*filedes,ultra);
		}
		else if(rx_data->thread_id == 4)
		{
			alco_on_flag = 0;
			printnumber(2);
			led_control(RED,OFF);
			alco = 5;
			printf("\alcohol sensor disconnected\n");	
			write_to_uart(*filedes,alco);
		}
		else if(rx_data->thread_id == 5)
		{
			ultra_on_flag = 0;
			printnumber(2);
			if(ultra_on_flag ==0 && alco_on_flag == 0)
			{
				printnumber(3);
			}
			led_control(RED,OFF);
			ultra = 0;
			printf("\nultrasonic sensor disconnected\n");	
			write_to_uart(*filedes,alco);
		}
		else
		{
			printnumber(4);
			led_control(RED,ON);
			static int addition;
			addition = addition + 50;
			addition = addition % 500;
			printf("\nuart disconnected\n");
			write_to_uart(*filedes,3);
			usleep(100);
			int ret_val = uart_init(PORT1,filedes);
			struct termios terminal_var;
			ret_val = termios_init(*filedes,&terminal_var);
		}
		return SUCCESS;
	}
	return 5;
}

