#ifndef __USER_LED_H
#define __USER_LED_H

#include <stdio.h>
#include <stdlib.h>

#define BLUE  1
#define RED   2
#define GREEN 3

#define ON  1
#define OFF 0

void led_control(int led_color, int status);

#endif
