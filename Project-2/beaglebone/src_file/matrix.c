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
* References:https://github.com/harsimran2421/Rtes_Final_Project/tree/master/led_mat
_._._._._._._._._._._._._._._._._._._._._.*/


/*user libraries*/
#include "matrix.h"

/*global variable*/
unsigned char disp1[11][8]={
  {0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
  {0x10,0x30,0x50,0x10,0x10,0x10,0x10,0x10},//1
  {0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2
  {0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3
  {0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4
  {0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5
  {0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6
  {0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7
  {0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
  {0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//9
};

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
static void Send16bits (unsigned short output)
{

  unsigned char i;

  for (i=16; i>0; i--) 
  {
    unsigned short mask = 1 << (i - 1); // calculate bitmask

    led_control(CLOCK, 0);  // set clock to 0

    // Send one bit on the data pin

    if (output & mask)   
      led_control(DATA, 1);          
    else                              
      led_control(DATA, 0);  

    led_control(CLOCK, 1);  // set clock to 1
  }
}


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
static void MAX7219Send (unsigned char reg_number, unsigned char dataout)
{
  led_control(LOAD, 1);  // set LOAD 1 to start
  Send16bits((reg_number << 8) + dataout);   // send 16 bits ( reg number + dataout )
  led_control(LOAD, 0);  // LOAD 0 to latch
  led_control(LOAD, 1);  // set LOAD 1 to finish
}


/* -------------------------------*/
/**
 * @Synopsis this function is used to initailly set up the ledmatrix
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
void setup_led()
{
  MAX7219Send(0x09, 0x00);
  MAX7219Send(0x0a, 0x10);
  MAX7219Send(0x0b, 0x07);
  MAX7219Send(0x0c, 0x01);
  MAX7219Send(0x0f, 0x00);
}

/* -------------------------------*/
/**
 * @Synopsis this function is used to print number on the led matrix 
 *
 * @Param OUTPUT is 
 *
 * @Returns void pointer is returned
 */
/* ---------------------------------*/
void printnumber(int i)
{
  int j;
  for(j = 1; j<9 ;j++)
  {
    MAX7219Send(j, disp1[i][j-1]);
  }
  usleep(50);
}
