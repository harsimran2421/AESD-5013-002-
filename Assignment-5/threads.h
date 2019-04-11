/*
 * threads.h
 *
 *  Created on: Apr 9, 2019
 *      Author: harsi
 */

#ifndef THREADS_H_
#define THREADS_H_

#include "FreeRTOS.h"
#include "main.h"
#include "queue.h"
#include"semphr.h"
#include<stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "timers.h"

typedef struct{
    int thread_id;
    TickType_t timestamp;
    char ip_str[20];
    double sensor_value;
}msg_struct;

xSemaphoreHandle log_m;
SemaphoreHandle_t led_signal;
SemaphoreHandle_t temp_signal;
TaskHandle_t a_task ;
QueueHandle_t queue_logger;

int queue_create();
void LEDTask(void *pvParameters);
void LoggerTask(void *pvParameters);
void TemperatureTask(void *pvParameters);
void timerTask(TimerHandle_t mytimer);
void temp_alert(void *pvParameters);
#endif /* THREADS_H_ */
