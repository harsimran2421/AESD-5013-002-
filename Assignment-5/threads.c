/*
 * threads.c
 *
 *  Created on: Apr 9, 2019
 *      Author: harsi
 *      reference: https://github.com/akobyl/TM4C129_FreeRTOS_Demo
 *      reference
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "threads.h"
#include "temperature.h"

//QueueHandle_t queue_logger;
int queue_create(){
    queue_logger = xQueueCreate( 10, sizeof(msg_struct) );
    if( queue_logger == NULL )
       {
           return 0;
       }
    return 1;
}


// Flash the LEDs on the launchpad
void LEDTask(void *pvParameters)
{
    msg_struct msg;
    uint32_t run_count;
    while(1)
    {
        if (xSemaphoreTake(led_signal, portMAX_DELAY) == pdTRUE)
         {
             LEDWrite(0x02, 0x02);
             LEDWrite(0x01, 0x01);
             vTaskDelay(pdMS_TO_TICKS(2));
             LEDWrite(0x02, 0x00);
             LEDWrite(0x01, 0x00);
             memset(msg.ip_str,'\0',sizeof(msg.ip_str));
             strcpy(msg.ip_str, "LED!!!!");
             msg.sensor_value = run_count++;
             msg.timestamp = xTaskGetTickCount();
             msg.thread_id = 2;

             if (xSemaphoreTake( log_m, ( TickType_t )20 ) == pdTRUE)

             {
                 xQueueSend(queue_logger, (void * ) &msg,
                            (TickType_t )1);
                 xSemaphoreGive(log_m);
             }

             vTaskDelay(pdMS_TO_TICKS(2));
         }
    }
}


// Write text over the Stellaris debug interface UART port
void LoggerTask(void *pvParameters)
{
    msg_struct msg;
    memset(msg.ip_str,'\0',sizeof(msg.ip_str));
    while(1)
    {
        {
            if (xQueueReceive(queue_logger, &(msg),(TickType_t ) 1) == pdTRUE)
            {
                if(msg.thread_id == 1)
                {
                    UARTprintf("\n\n\rTimestamp: %umsec\nAuthor: Harry\nALERT TASK:%s\n\r ",msg.timestamp,msg.ip_str);
                }
                else if(msg.thread_id == 3)
                {
                    UARTprintf("\n\n\rTimestamp:%u msec\n%s\nAuthor: Harry\nSensor value = %u Celsius\n\r ",msg.timestamp,msg.ip_str,
                            (uint32_t) msg.sensor_value);
                }
                else if(msg.thread_id == 2)
                {
                    UARTprintf("\n\n\rTimestamp:%u msec\n%s\nAuthor: Harry\nCount = %u \n\r ",msg.timestamp,msg.ip_str,(uint32_t) msg.sensor_value);
                }
             }
         }
    }
}

void TemperatureTask(void *pvParameters)
{
    msg_struct msg;
    double temp_value;
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        if (xSemaphoreTake(temp_signal,portMAX_DELAY) == pdTRUE)
        {
            temp_main(CELCIUS, &temp_value);
            if (temp_value >= 25)
            {
                xTaskNotifyGive(a_task);

            }
            memset(msg.ip_str,'\0',sizeof(msg.ip_str));
            msg.sensor_value = temp_value;
            msg.thread_id = 3;
            strcpy(msg.ip_str, "Temperature Task");
            msg.timestamp = xTaskGetTickCount();
            if (xSemaphoreTake( log_m, ( TickType_t )20 ) == pdTRUE)
                ;
            {
                xQueueSend(queue_logger, (void * ) &msg, (TickType_t )1);
                xSemaphoreGive(log_m);
            }
        }
    }
}

void timerTask(TimerHandle_t mytimer)
{
    static int run_count;
    xSemaphoreGive(led_signal);
    run_count++;
    if(run_count%10 == 0)
    {
        xSemaphoreGive( temp_signal);
    }
}

void temp_alert(void *pvParameters)
{
    msg_struct msg;
    memset(msg.ip_str,'\0',sizeof(msg.ip_str));
    for(;;)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
        strcpy(msg.ip_str, "Temperature alert");
        msg.sensor_value = 0;
        msg.thread_id = 1;
        msg.timestamp = xTaskGetTickCount();
        if (xSemaphoreTake( log_m, portMAX_DELAY) == pdTRUE)
            ;
        {
            xQueueSend(queue_logger, (void * ) &msg,
                       (TickType_t )1);
            xSemaphoreGive(log_m);
        }
    }
}
