
/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : main_sort.c
* Creation Date : 9-02-2019
* Last Modified : Fri 9 Feb 2019 00:00:56 PM MDT
* Created By : Harsimransingh
* Description: Source file for harry_sort functions system call
_._._._._._._._._._._._._._._._._._._._._.*/



#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/harry_sort.h>


#include <stdint.h>

/*reference geeks for geeks website*/
static void merge_elements(int32_t arr[], int32_t left, int32_t middle, int32_t right)
{
  int32_t i, j, k;
  int32_t val1 = middle -left + 1;
  int32_t val2 = right - middle;
  int32_t x=0,y=0;
  int32_t left_arr[val1], right_arr[val2];
  for( x = 0; x<val1; x++)
  {
    left_arr[x] = arr[left+x];
  }
  for(y = 0; y< val2 ; y++)
  {
    right_arr[y] = arr[middle+y+1];
  }
  k = left;

  i = 0;
  j = 0;
  while(i < val1 && j < val2)
  {
    if(left_arr[i] > right_arr[j])
    {
      arr[k] = left_arr[i];
      i++;
    }
    else
    {
      arr[k] = right_arr[j];
      j++;
    }
    k++;
  }
  while(i<val1)
  {
    arr[k] = left_arr[i];
    i++;
    k++;
  }

  while(j<val2)
  {
    arr[k] = right_arr[j];
    j++;
    k++;
  }

}

/*reference geeks for geeks website*/
static void merge_sort(int32_t arr[], int32_t left, int32_t right)
{
  if(left<right)
  {
    //To calculate the midpoint for the array index/
    uint32_t middle = left + (right - left)/2;

    //to divide the uper half of the array further/
    merge_sort(arr, left, middle);

    //to divide the uper half of the array further/
    merge_sort(arr, middle + 1, right);

    merge_elements(arr, left, middle, right);
  }
}

/*user defined system call to sort an array*/
SYSCALL_DEFINE3(harry_sort, int32_t __user*, src_ptr, int, sort_length, int32_t __user*, dest_ptr)
{
 int32_t *buff = NULL;
 int32_t i = 0;
 printk(KERN_INFO "ENTERED SYSCALL SUCCESSFULLY!!!!!!! \n");
 buff = (int32_t*)kmalloc(sizeof(int32_t)*sort_length, GFP_KERNEL);
 if(buff == NULL)
 {
	printk(KERN_ALERT "Malloc failed.\n");
	return -ENOMEM;
 } 
 
  if (src_ptr == NULL)
  {
       printk(KERN_ERR "Null pointer error \n");
	     kfree(buff);
       return -EINVAL;
  }
  else if (dest_ptr == NULL)
  {
       printk(KERN_ERR "Null pointer error \n");
	     kfree(buff);
       return -EINVAL;
  }
  else if(sort_length < 256)
  {
    printk(KERN_DEBUG "Buffer size < 255 \n");
    kfree(buff);
    return -EINVAL;
  }
  else
  {

    if (copy_from_user(buff, src_ptr, (sizeof(int32_t)*sort_length)))  //copy from user space to kernal space
    {
      kfree(buff);
      return -EFAULT;
    }
    printk(KERN_INFO"Starting to sort!\n");   
    merge_sort(buff,0,sort_length-1);  //sorting function
    printk(KERN_INFO"Done sorting\n");

    if (copy_to_user(dest_ptr, buff, (sizeof(int32_t)*sort_length)))  //copy from kernel space to user space
    {
        kfree(buff);
        return -EFAULT;
    }
  }
  kfree(buff);
return 0;
}


