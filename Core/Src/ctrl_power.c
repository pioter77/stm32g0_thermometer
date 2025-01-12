/*
 * ctrl_power.c
 *
 *  Created on: Jan 11, 2025
 *      Author: x280
 */


#include "ctrl_power.h"
#include "stm32g0xx_ll_pwr.h"
#include "ssd1306.h"



CTRL_Power_t CTRLpower={
		.currentMode= CTRL_POWER_MODE_RUN,
		.nextMode= CTRL_POWER_MODE_RUN,
		.enterNextPowerMode= 0,
		.wkupPin_L= LL_PWR_WAKEUP_PIN1,
		.wkupPin_M= LL_PWR_WAKEUP_PIN4,
		.wkupPin_R= LL_PWR_WAKEUP_PIN2,
};


void ctrl_power_init(CTRL_Power_t *ctrl)
{
	//default mode after bootup is run mode
	ctrl->currentMode = CTRL_POWER_MODE_RUN;
	ctrl->nextMode = CTRL_POWER_MODE_STANDBY;
	ctrl->enterNextPowerMode = 0;

	if(LL_PWR_IsActiveFlag_SB())
	{
		LL_PWR_ClearFlag_SB();		//if this flag is set this means that code starts executing from standby wakeup mode
	}

}

void ctrl_power_setWakeUpPins(CTRL_Power_t *ctrl)
{
	LL_PWR_EnableWakeUpPin(ctrl->wkupPin_L);
	LL_PWR_EnableWakeUpPin(ctrl->wkupPin_M);
	LL_PWR_EnableWakeUpPin(ctrl->wkupPin_R);
	LL_PWR_SetWakeUpPinPolarityLow(ctrl->wkupPin_L);
	LL_PWR_SetWakeUpPinPolarityLow(ctrl->wkupPin_M);
	LL_PWR_SetWakeUpPinPolarityLow(ctrl->wkupPin_R);
}

void ctrl_power_setNextMode(CTRL_Power_t *ctrl)
{

}

void ctrl_power_enterNextMode(CTRL_Power_t *ctrl)
{

}

void ctrl_power(CTRL_Power_t *ctrl)
{
	if(ctrl->enterNextPowerMode)
	{
		if(ctrl->currentMode != ctrl->nextMode)
		{
			switch(ctrl->nextMode){

				case CTRL_POWER_MODE_STANDBY:
					ctrl->enterNextPowerMode = 0;
					ctrl->currentMode = ctrl->nextMode;
					LL_RCC_LSE_Enable();
					LL_PWR_ClearFlag_WU();						//clear all wakeup flags all 4 of them
					ctrl_power_setWakeUpPins(ctrl);
					SSD1306_OFF(I2C1);
					LL_mDelay(2000);							//give some time to gpio reconfiguration and prevent from sticking wakup pin
					LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);	//LPMS = 011 //select standby mode
					LL_LPM_EnableDeepSleep();					//SLEEPDEEP bit set in cortex m0 system control reg
					LL_GPIO_TogglePin(LED_OUT_GPIO_Port, LED_OUT_Pin);	//diode off
					__WFI();
					break;
				default:
		//			ctrl->nextMode == CTRL_POWER_MODE_RUN
					break;
			}
		}else{
			//we are already in the requested mode - just do nothing
		}
	}
}
