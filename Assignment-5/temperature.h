/*
 * temperature.h
 *
 *  Created on: Apr 10, 2019
 *      Author: harsi
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_
#include "main.h"
#include "harry_i2c.h"
#include "threads.h"

typedef enum{FAHRENHEIT, KELVIN, CELCIUS}conversion_enum;
status_enum temp_main(conversion_enum request, double *temp_value);

#endif /* TEMPERATURE_H_ */
