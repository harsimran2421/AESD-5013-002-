/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : matrix.h
* Creation Date :28-04-2019
* Last Modified : Sun 28 April 2019 20:10:11 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: matrix file for interfacing LED Matrix function declaration and structure declarations
* 
* Functions:
*           static void Send16bits () this function is used to write all the individual pixels 
*           static void MAX7219() Send is send function to write anything to the LED Matrix 
* 	    void setup_led() this function is used to initailly set up the ledmatrix
*           void printnumber() thisfunction is used to print number on the led matrix 
* References:
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef MATRIX_H
#define MATRIX_H

/*standard libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h> 
#include <semaphore.h>

/*user libraries*/
#include "user_led.h"

/*macros*/
#define DECODE_MODE   0x09                       
#define INTENSITY     0x0a                        
#define SCAN_LIMIT    0x0b                        
#define SHUTDOWN      0x0c                        
#define DISPLAY_TEST  0x0f                         

/* -------------------------------*/
/**
 * @Synopsis this function is used to write all the individual pixels 
 *
 * @Param OUTPUT is a variable to carry out the output value from the function
 * 
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
static void Send16bits (unsigned short output);

/* -------------------------------*/
/**
 * @Synopsis this function is send function to write anything to the LED Matrix  
 *
 * @Param reg_number is the configuration input
 * @Param dataout is value of the individual pixel to be written
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
static void MAX7219Send (unsigned char reg_number, unsigned char dataout);

/* -------------------------------*/
/**
 * @Synopsis this function is used to initailly set up the ledmatrix
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
void setup_led();

/* -------------------------------*/
/**
 * @Synopsis this function is used to print number on the led matrix 
 *
 * @Param OUTPUT is 
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
void printnumber(int i);

#endif
