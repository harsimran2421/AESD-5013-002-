/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : hw3_question_4.c
* Creation Date : 13-02-2019
* Last Modified : Fri 16 Feb 2019 00:00:56 PM MDT
* Created By : Harsimransingh
* Description: Source file for sorting animal array and creating 2 lists based of filters and non repeating animals
_._._._._._._._._._._._._._._._._._._._._.*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/string.h> 
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Harsimransingh Bindra");
MODULE_DESCRIPTION("This is a basic kernal module for sorting array of animal names and apply different filters ");

static int count = 0;
static char *animal_name = "DEFAULT";

module_param(count, int, S_IWUSR | S_IXUSR | S_IRUSR);
MODULE_PARM_DESC(count, "animal count");
module_param(animal_name, charp, S_IWUSR | S_IXUSR | S_IRUSR);
MODULE_PARM_DESC(animal_name, "animal name");


enum error_message
{
  ERROR = -1,
  SUCCESS,
  EMPTY_LIST,
  REPEAT
};

enum POSITION
{
  END = -1,
  START = 1
};

struct user_node {
  struct list_head head_list;
  char data[10];
  int count;
};

struct user_node list1, list2;


static int string_check(char *str1,char *str2)
{
  int i = 0;
  while (*(str1+ i) != '\0' && *(str2 + i) != '\0')
  {
    if(*(str1+ i) < *(str2+ i))
    {
      return 0;
    }
    else if(*(str1+ i) > *(str2+ i))
    {
      return 1;
    }
    else
    {
      i++;
    }
  }
  return 0;
}

static void sort(char* arr[], int n)
{
  char *temp;
  int i = 0, j = 0;
  for(i = 0; i<50 ;i++)
  {
    for(j = i+1; j<50; j++)
    {
      if(string_check(arr[i],arr[j])==1)
      {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
}

static int node_insert(struct list_head *head,char data[])
{
    struct user_node *new_node = (struct user_node*)kmalloc(sizeof(struct user_node),GFP_KERNEL);
    strcpy(new_node->data, data);
    new_node->count = 1;
    INIT_LIST_HEAD(&new_node->head_list);
    list_add_tail(&(new_node->head_list),head);
    return SUCCESS;
}

static int node_insert_list2(struct list_head *head,char data[], int animal_count)
{
    struct user_node *new_node = (struct user_node*)kmalloc(sizeof(struct user_node),GFP_KERNEL);
    strcpy(new_node->data, data);
    new_node->count = animal_count;
    INIT_LIST_HEAD(&new_node->head_list);
    list_add_tail(&(new_node->head_list),head);
    return SUCCESS;
}

static void print_list1(struct user_node *head)
{
  int i;
  i = 1;
  struct user_node *print_list;
  list_for_each_entry(print_list,&(head->head_list),head_list)
  {
    printk("Node value at position %d \t is %s and count is: %d\n",i,print_list->data,print_list->count);
    i++;
  }
}

static int __init my_init(void)
{
  printk(KERN_DEFAULT "Module inserted \n");
  char* arr[]={"donkey","monkey","dog","cat","elephant","tiger","snake","spider","donkey","monkey","dog","cat","elephant","tiger","snake","spider","donkey","monkey","dog","cat","elephant","tiger","snake","spider","donkey","monkey","dog","cat","elephant","tiger","snake","spider","donkey","monkey","dog","cat","elephant","tiger","snake","spider","donkey","monkey","dog","cat","elephant","tiger","snake","spider","panda","rabbit"};
  int i = 0;
  int arr_length;
  int present_flag = 0;
  int number_of_elements = 0, l2_number_of_elements = 0;
  struct user_node *temp_list;
  arr_length = sizeof(arr)/sizeof(arr[0]);
  sort(arr,arr_length);
  for (i = 0; i < arr_length; i++) 
        printk("%d: %s \n", i, arr[i]);
	
  INIT_LIST_HEAD(&(list1.head_list));
  
  for(i = 0; i < arr_length; i++)
  {
    list_for_each_entry(temp_list,&(list1.head_list),head_list)
    {
      if(strcmp(temp_list->data,arr[i]) == 0)
      {
        temp_list->count++;
        present_flag++;
        break;
      }
    }
    if(present_flag == 0)
    {
      node_insert(&(list1.head_list),arr[i]);
      number_of_elements++;
    }
    present_flag = 0;
  }
  printk("Size occupied by list 1 is :%d\n",number_of_elements*sizeof(struct user_node));

  print_list1(&list1);

  printk("\n\nSET 2\n\n");

  INIT_LIST_HEAD(&(list2.head_list));
  
  if(strcmp(animal_name,"DEFAULT") == 0)
  {
    list_for_each_entry(temp_list,&(list1.head_list),head_list)
    {
      if(temp_list->count > count)
      {
        node_insert_list2(&(list2.head_list),temp_list->data,temp_list->count);
        l2_number_of_elements++;
      }
    }
  
  }
  else
  {
    list_for_each_entry(temp_list,&(list1.head_list),head_list)
    {
      if(temp_list->count > count)
      {
        if(strcmp(temp_list->data,animal_name) == 0)
        {
          node_insert_list2(&(list2.head_list),temp_list->data,temp_list->count);
          l2_number_of_elements++;
        }
      }
    }
  }
  printk("\nSET 2 list with name filter: %s and animal count greater than: %d\n",animal_name, count);
  print_list1(&list2);

  list_for_each_entry(temp_list, &(list1.head_list), head_list)
	{
		kfree(temp_list);
	}
  list_for_each_entry(temp_list, &(list2.head_list), head_list)
	{
		kfree(temp_list);
	}
	printk("Size freed by Set1: %d\n",number_of_elements*sizeof(struct user_node));
	printk("Size freed by Set1: %d\n",l2_number_of_elements*sizeof(struct user_node));
  return 0;
}

static void __exit my_exit(void)
{
  printk(KERN_DEFAULT "Module removed\n");
}

module_init(my_init);
module_exit(my_exit);
