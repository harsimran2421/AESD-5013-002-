/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : main_sort.c
* Creation Date : 9-02-2019
* Last Modified : Fri 9 Feb 2019 00:00:56 PM MDT
* Created By : Harsimransingh
* Description: Source file to test system call
_._._._._._._._._._._._._._._._._._._._._.*/



#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
	
  int i, n, j;
  time_t t;
  int32_t src_arr[256], dest_arr[256];

  printf("harry_sort syscall is action\n");

  /* Intializes random number generator */
  srand((unsigned) time(&t));

  /* Print 5 random numbers from 0 to 49 */
  for( i = 0 ; i <256 ; i++ ) {
     src_arr[i] = 48 + (rand() % 79);
  }
 
  for( i = 0 ; i <256 ; i++ ) {
     printf("%c", src_arr[i]);
  }
  printf("/n");
  int32_t *src_str = src_arr;
  int32_t *sort_list = dest_arr;

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

