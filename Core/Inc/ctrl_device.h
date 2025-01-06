/*
 * ctrl_device.h
 *
 *  Created on: Jan 6, 2025
 *      Author: x280
 */

#ifndef INC_CTRL_DEVICE_H_
#define INC_CTRL_DEVICE_H_

#include "main.h"

typedef struct CTRL_Device_T{
	volatile uint16_t temp_int_raw;				//internal temperature of the uC, raw 16 bit format as read from adc
	volatile uint16_t vBat_raw;					//battery voltage level, raw 16 bit format as read from adc
}CTRL_Device_t;

extern CTRL_Device_t CTRLdevice;


void ctrl_device(CTRL_Device_t *ctrl);
#endif /* INC_CTRL_DEVICE_H_ */
