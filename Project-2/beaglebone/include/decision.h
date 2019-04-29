#ifndef DECISION_H
#define DECISION_H

#include "uart.h"
#include "logger.h"

#define ALCOHOL 2
#define ULTRASONIC 3

#define ALCOHOL_THRESHOLD 100
#define ULTRASONIC_THRESHOLD 10

void *decision(void *arg);

#endif
