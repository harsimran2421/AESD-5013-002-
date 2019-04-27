#ifndef DECISION.H
#define DECISION.H

#include "logger.h"
#include "uart.h"

#define ALCOHOL 1
#define ULTRASONIC 2
#define ALCOHOL_THRESHOLD 20
#define ULTRASONIC_THRESHOLD 20


typedef struct decsion_struct_send
{
  int thread_name;
  int decision;
} dec_struct_send;

typedef struct decision_struct_recv
{
  int thread_id;
  float sensor_value;
} dec_struct_recv;

void *decision(void);
#endif
