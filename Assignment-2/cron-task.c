#include <stdio.h>
#include <linux/kernel.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int main()
{
  int i, n, j;
  time_t t;
  int32_t src_arr[256], dest_arr[256];
  int32_t temp;
  time_t now;	
  int32_t *src_str = src_arr;
  int32_t *sort_list = dest_arr;
  struct timeval tv;
  long int process_id = getpid();
  long int user_id = getuid();
	printf("Current process ID= %ld\n", process_id);
	printf("Current user ID= %ld\n", user_id);

	//time(&now);
  gettimeofday(&tv,NULL);
	printf("Today is : %s\n", ctime(&tv.tv_sec));

  sleep(1);
  /* Intializes random number generator */

  srand((unsigned) time(&t));

  for( i = 0 ; i <256 ; i++ ) {
     src_arr[i] = 48 + (rand() % 79);
  }
 
  printf("Input string is:");
  for( i = 0 ; i <256 ; i++ ) {
     printf("%c", src_arr[i]);
  }
  printf("/n");

  int ret = syscall(398, src_str, 256, sort_list);

  printf("\n");
  if (ret == 0)
	{
		printf("\nAnd its a success!!!!!\n");
	}
	else	printf("\nFailed better luck next time\n");

  for( i = 0 ; i <256 ; i++ ) {
     printf("%c", *(sort_list+i));
  }
printf("\n");  

return 0;
}
