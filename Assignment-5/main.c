/*
 * Refernces:
 * FreeRTOS 8.2 Tiva Demo
 *
 * main.c
 *
 * Andy Kobyljanec
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
 I2C references:
 https://e2e.ti.com/support/microcontrollers/other/f/908/t/468677
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"



// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "threads.h"
#include "harry_i2c.h"
#include "timers.h"


void configure_UART(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 9600, 16000000);
}


// Main function
int main(void)
{
    led_signal = xSemaphoreCreateBinary();
    temp_signal = xSemaphoreCreateBinary();
    log_m = xSemaphoreCreateMutex();
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);
    PinoutSet(false, false);
    queue_create();
    configure_UART();

    GPIO_Init();
    I2C_Init();

    TIMER_Init();

    xTaskCreate(LEDTask, (const portCHAR *)"LED", 128, NULL, 1, NULL);

    xTaskCreate(LoggerTask, (const portCHAR *)"Serial", 128, NULL, 1, NULL);

    xTaskCreate(TemperatureTask, (const portCHAR *)"Temperature", 128, NULL, 1, NULL);

    xTaskCreate(temp_alert, (const portCHAR *)"ALERT", 128, NULL, 2,  &a_task);

    vTaskStartScheduler();
    return 0;
}

void TIMER_Init()
{
       TimerHandle_t myTimer = NULL;
       myTimer = xTimerCreate(
               "Timer1",
               pdMS_TO_TICKS(100),
               pdTRUE,
               (void *) pvTimerGetTimerID(myTimer),
               timerTask
             );
      xTimerStart(myTimer, 0);
}

void GPIO_Init()
{
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
     GPIOPinConfigure(GPIO_PL1_I2C2SCL);
     GPIOPinConfigure(GPIO_PL0_I2C2SDA);
     GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
     GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
}

/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}


