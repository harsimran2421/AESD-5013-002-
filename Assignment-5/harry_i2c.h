/*
 * harry_i2c.h
 *
 *  Created on: Apr 9, 2019
 *      Author: harsi
 */

#ifndef HARRY_I2C_H_
#define HARRY_I2C_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"

uint32_t sys_clock;

typedef enum{SUCCESS, FAILURE}status_enum;

void I2C_Init();

status_enum i2c_read(uint8_t slave_addr,uint8_t reg_addr, uint8_t *data);

#endif /* HARRY_I2C_H_ */
