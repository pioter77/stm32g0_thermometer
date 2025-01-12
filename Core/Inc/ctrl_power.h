/*
 * ctrl.power.h
 *
 *  Created on: Jan 11, 2025
 *      Author: x280
 */

#ifndef INC_CTRL_POWER_H_
#define INC_CTRL_POWER_H_

#include "main.h"
//#include "stm32g0xx_ll_gpio.h"
#include "gpio.h"

typedef enum {
		CTRL_POWER_MODE_RUN=0,
		CTRL_POWER_MODE_SLEEP,
		CTRL_POWER_MODE_LOW_POWER_RUN,
		CTRL_POWER_MODE_LOW_POWER_SLEEP,
		CTRL_POWER_MODE_STOP_0,
		CTRL_POWER_MODE_STOP_1,
		CTRL_POWER_MODE_STANDBY,
		CTRL_POWER_MODE_SHUTDOWN,

}ctrl_power_powermode;

typedef struct CTRL_Power_T{
	ctrl_power_powermode currentMode;
	ctrl_power_powermode nextMode;
	const uint32_t wkupPin_L;
	const uint32_t wkupPin_M;
	const uint32_t wkupPin_R;
	volatile _Bool enterNextPowerMode;	//flag to switch to next mode by isr
}CTRL_Power_t;

extern CTRL_Power_t CTRLpower;

void ctrl_power(CTRL_Power_t *ctrl);
void ctrl_power_init(CTRL_Power_t *ctrl);
void ctrl_power_setWakeUpPin(CTRL_Power_t *ctrl, GPIO_TypeDef *port, uint32_t pin);
void ctrl_power_setNextMode(CTRL_Power_t *ctrl);
void ctrl_power_enterNextMode(CTRL_Power_t *ctrl);

#endif /* INC_CTRL_POWER_H_ */
