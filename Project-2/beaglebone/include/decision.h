/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : decision.h
* Creation Date :28-04-2019
* Last Modified : Sun 28 April 2019 20:10:11 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: decision file for decision function declaration and structure declarations
* 
* Functions:
*           decision() function to set decisio flags based on incoming sensor values
* References:
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef DECISION_H
#define DECISION_H


/*user libraries*/
#include "uart.h"
#include "logger.h"
#include "user_led.h"
#include "matrix.h"

/*macros*/
#define ALCOHOL 2
#define ULTRASONIC 3
<<<<<<< HEAD
#define ALCOHOL_THRESHOLD 100
#define ULTRASONIC_THRESHOLD 10

/* -------------------------------*/
/**
 * @Synopsis decision task to set the decision flags for different sensors and communciate back the 
 * flag values to the Remote node using UART
 *
 * @Param arg to pass arguement to pthread 
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
=======
#define TEMPERATURE 10

#define ALCOHOL_THRESHOLD 100
#define ULTRASONIC_THRESHOLD 10
#define TEMPERATURE_THRESHOLD 30
>>>>>>> 242653d7b9de5f9321ad8670fd6c200c86687a7f
void *decision(void *arg);

#endif
