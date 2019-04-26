#include "uart.h"


void main()
{
  int file_descriptor = 0;
	int ret_val = uart_init(PORT1,&file_descriptor);
  struct termios terminal_var;
	printf("ret_val for uart_init = %d", ret_val);

	ret_val = termios_init(file_descriptor,&terminal_var);
	printf("ret_val for terminos_init = %d", ret_val);

	while(1)
	{
	//write_to_uart(file_descriptor);
	read_from_uart(file_descriptor);
	sleep(1);
	 }
}

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

	//fcntl(*file_descriptor, F_SETFL, FNDELAY);
	return SUCCESS;
}

int termios_init(int file_descriptor, struct termios *term)
{
	tcgetattr(file_descriptor, term);

  term->c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON); 
	//term->c_iflag = 0;

	//term->c_oflag &= ~OPOST;
  term->c_oflag = 0;
	term->c_lflag &= ~(ECHO | ECHONL | ICANON |/* ECHOK | ECHOE |*/ ISIG | IEXTEN);	

	//term->c_cflag = (CS8 | CLOCAL | CREAD | B9600);

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

uart_status read_from_uart(int filedes)
{
	int ret_val;
	msg_struct rx_data;
	int filedes1;
	if(ret_val = read(filedes, &rx_data, sizeof(rx_data)) <1)
	{
		perror("Error Reading data\n");
		return RECEIVE_ERROR;
	}
	else
	{
		//printf("data Reading success\n");
		printf("Received data = %s\n", rx_data.ip_str);
		printf("sensor value = %f\n", rx_data.sensor_value);
		printf("task id = %d\n\n", rx_data.thread_id);
		return SUCCESS;
	}
	return 5;
}

