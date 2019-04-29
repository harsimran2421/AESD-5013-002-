#include "uart.h"

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


uart_status read_from_uart(int *filedes,msg_struct *rx_data)
{
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
			ultra = ultra ^ 1;
			printf("\nalcohol task \t sensor value = %f\n", rx_data->sensor_value);
			write_to_uart(*filedes,ultra);
		}		
		else if(rx_data->thread_id == 3)
		{
			alco = alco ^ 5;
			printf("\nultrasonic task \t sensor value = %f\n", rx_data->sensor_value);	
			write_to_uart(*filedes,alco);
		}
		else if(rx_data->thread_id == 4)
		{
			alco = alco ^ 5;
			printf("\alcohol sensor disconnected\n");	
			write_to_uart(*filedes,alco);
		}
		else if(rx_data->thread_id == 5)
		{
			ultra = ultra ^ 1;
			printf("\nultrasonic sensor disconnected\n");	
			write_to_uart(*filedes,alco);
		}
		else
		{
			static int addition;
			addition = addition + 50;
			addition = addition % 500;
			printf("\nincorrect value %d\n",addition);
			write_to_uart(*filedes,3);
			usleep(100);
			int ret_val = uart_init(PORT1,filedes);
			struct termios terminal_var;
			printf("ret_val for uart_init = %d", ret_val);
			ret_val = termios_init(*filedes,&terminal_var);
			printf("ret_val for terminos_init = %d", ret_val);
		}
		return SUCCESS;
	}
	return 5;
}

