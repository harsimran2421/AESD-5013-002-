/*
 * temperature.c
 *
 *  Created on: Apr 9, 2019
 *      Author: harsi
 */
#include "temperature.h"
status_enum temp_main(conversion_enum request, double *temp_value)
{
    int32_t converted_value = 0;
    uint8_t sensor_val[2];
    if(i2c_read(0x48, 0x00, sensor_val) != SUCCESS)
    {
        return FAILURE;
    }

    if (*(sensor_val + 0) & 0x01)
    {

        converted_value = (*(sensor_val + 1) << 5) | (*(sensor_val + 0) >> 3);
        if (converted_value > 0xFFF)
        {
            converted_value |= 0xE000;
        }
    }
    else
    {
        converted_value = (*(sensor_val + 1) << 4) | (*(sensor_val + 0) >> 4);
        if (converted_value > 0x7FF)
        {
            converted_value |= 0xF000;
        }
    }
        *temp_value = (converted_value * 0.0625);

    return SUCCESS;
}
