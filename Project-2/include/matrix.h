#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h> 
#include <semaphore.h>
#include "user_led.h"

#define DECODE_MODE   0x09                       
#define INTENSITY     0x0a                        
#define SCAN_LIMIT    0x0b                        
#define SHUTDOWN      0x0c                        
#define DISPLAY_TEST  0x0f                         

static void Send16bits (unsigned short output);

static void MAX7219Send (unsigned char reg_number, unsigned char dataout);

void setup_led();

void printnumber(int i);

#endif
