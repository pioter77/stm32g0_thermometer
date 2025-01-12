/*
 * ctrl_device.c
 *
 *  Created on: Jan 6, 2025
 *      Author: x280
 */

#include "ctrl_device.h"
#include "fonts.h"
#include "ctrl_rtc.h"
#include "ctrl_menu.h"
#include "ctrl_buttons.h"
#include "ctrl_measure.h"
#include "ssd1306.h"
#include "ctrl_power.h"

CTRL_Device_t CTRLdevice = {
		.temp_int_raw = 0,
		.vBat_raw = 0
};


void ctrl_device(CTRL_Device_t *ctrl)
{
	  ctrl_power(&CTRLpower);
	  rtc_update();
	  ctrl_measure();
	  ctrl_buttons(&CTRLbuttons);
	  ctrl_menu(&CTRLmenu);
	  ssd1306_UpdateScreen(I2C1);
}
