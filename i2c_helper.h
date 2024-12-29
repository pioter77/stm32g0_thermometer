#ifndef _I2C_HELPER_H
#define _I2C_HELPER_H

#include "stm32g0xx_ll_i2c.h"
#include "stm32g0xx_ll_gpio.h"

#define I2C_REQUEST_WRITE   0x00
#define I2C_REQUEST_READ    0x01

void I2C_WriteData(I2C_TypeDef *I2Cx,uint8_t Address, uint8_t Reg, uint8_t Data);
void I2C_ReadData(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
void I2C_MultWriteData(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t *Data, uint16_t size);

void I2C_Start(I2C_TypeDef *I2Cx);
void I2C_Write(I2C_TypeDef *I2Cx, uint8_t data);
void I2C_MultWrite(I2C_TypeDef *I2Cx, uint8_t *data, uint16_t size);
void I2C_Read(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t *buffer, uint8_t size);
void I2C_Address(I2C_TypeDef *I2Cx,uint8_t Address);
void I2C_Stop(I2C_TypeDef *I2Cx);

#endif
