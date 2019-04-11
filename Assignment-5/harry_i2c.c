/*
 * harry_i2c.c
 *
 *  Created on: Apr 9, 2019
 *      Author: harsi
 */
#include "harry_i2c.h"

void I2C_Init(void)
{

        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
             GPIOPinConfigure(GPIO_PL1_I2C2SCL);
            GPIOPinConfigure(GPIO_PL0_I2C2SDA);
            GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
            GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
            SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
            I2CMasterInitExpClk(I2C2_BASE,120000000U , true);

}

status_enum i2c_read(uint8_t slave_addr,uint8_t reg_addr, uint8_t *data)
{


    I2CMasterSlaveAddrSet(I2C2_BASE, slave_addr, false);

    I2CMasterDataPut(I2C2_BASE, 0x00);

    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(!I2CMasterBusy(I2C2_BASE));
    while(I2CMasterBusy(I2C2_BASE));
    uint32_t resp;
     resp = ROM_I2CMasterErr(I2C2_BASE);
     if(resp != I2C_MASTER_ERR_NONE)
     {
         return FAILURE;
     }
    I2CMasterSlaveAddrSet(I2C2_BASE, 0x48, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(!I2CMasterBusy(I2C2_BASE));
    while (I2CMasterBusy(I2C2_BASE));
    resp = I2CMasterErr(I2C2_BASE);
    if(resp != I2C_MASTER_ERR_NONE)
    {
        I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
        return FAILURE;
    }
    else
    {
    *(data + 1) = I2CMasterDataGet(I2C2_BASE);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    while(!I2CMasterBusy(I2C2_BASE));
    while (I2CMasterBusy(I2C2_BASE));
    resp = I2CMasterErr(I2C2_BASE);
    if(resp != I2C_MASTER_ERR_NONE)
    {

        I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
        return FAILURE;
    }
    else
    {

        *(data + 0) = (uint8_t)I2CMasterDataGet(I2C2_BASE);
        I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    }
    return SUCCESS;
    }
}
