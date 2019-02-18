/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : kernal_mod.c
* Creation Date : 13-02-2019
* Last Modified : Fri 16 Feb 2019 00:00:56 PM MDT
* Created By : Harsimransingh
* Description: Source file for kernal module to run a kernal timer
_._._._._._._._._._._._._._._._._._._._._.*/

/*reference: https://www.ibm.com/developerworks/library/l-timers-list/index.html
 *           https://stackoverflow.com/questions/4039602/example-for-kernel-timer-implementation-in-linux-in-kernel-2-6-32
 *           */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/stat.h>


MODULE_AUTHOR("Harsimransingh Bindra");
MODULE_DESCRIPTION("This is a basic kernal module developed to wake up a kernal timer every 500 msec by default");

static int counter = 0;
static char *user_name = "Harry";
static int timeout_value = 500;

module_param(timeout_value, int, S_IWUSR | S_IXUSR | S_IRUSR);
MODULE_PARM_DESC(timeout_value, "Kernel Timeout");
module_param(user_name, charp, S_IWUSR | S_IXUSR | S_IRUSR);
MODULE_PARM_DESC(user_name, "User's name");

struct timer_list user_timer;

static void trigger_function(void)
{   counter++;
    printk(KERN_INFO "Name: %s, Triggered count: %d", user_name, counter);
    mod_timer(&user_timer, jiffies + msecs_to_jiffies(timeout_value));
}

static int __init my_module_init(void)
{
    printk(KERN_INFO "my_module_init\n");
    init_timer(&user_timer);
    user_timer.data = (unsigned long)0;
    user_imer.function = (void(*)(unsigned long))trigger_function;
    user_timer.expires = jiffies + msecs_to_jiffies(timeout_value);
    add_timer(&user_timer);
    return 0;
}

static void __exit my_module_exit(void)
{
    printk("Exiting my module\n");
    del_timer(&user_timer);
    return;
    
}

module_init(my_module_init);
module_exit(my_module_exit);
