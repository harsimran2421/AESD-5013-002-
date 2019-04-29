#ifndef DECISION_H
#define DECISION_H

#include "uart.h"
#include "logger.h"
#include "user_led.h"
#include "matrix.h"

#define ALCOHOL 2
#define ULTRASONIC 3
#define TEMPERATURE 10

#define ALCOHOL_THRESHOLD 100
#define ULTRASONIC_THRESHOLD 10
#define TEMPERATURE_THRESHOLD 30
void *decision(void *arg);

#endif
