/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : harr_sort.h
* Creation Date : 8-02-2019
* Last Modified : Fri 8 Feb 2019 11:53:56 PM MDT
* Created By : Harsimransingh
* Description: header file for harry_sort system call
_._._._._._._._._._._._._._._._._._._._._.*/

#ifndef _LINUX_HARRY_SORT_H
#define _LINUX_HARRY_SORT_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscall.h>
#include <linux/errno.h>

int harry_sort(int32_t *src_ptr, int sort_length, int32_t *dest_ptr);

#endif
