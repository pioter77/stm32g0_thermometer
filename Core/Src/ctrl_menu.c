/*
 * ctrl_menu.c
 *
 *  Created on: Jan 6, 2025
 *      Author: x280
 */

#include "ctrl_menu.h"
#include "ssd1306.h"
#include "ctrl_rtc.h"
#include "fonts.h"
#include "stdio.h"
#include "ctrl_device.h"

CTRL_Menu_t CTRLmenu={
	.menuPos = 2,
	.menuMax = 3,
	.drawMainPanels = 1
};

static void draw_panel_climate(void)
{
	  const char buf1[] = "I:+24.5*C";
	  const char buf2[] = "O:-12.9*C";
	  const char buf0[] = "Sht21";
	  ssd1306_DrawText(buf0, Font6x8, 28, 24, White);
	  ssd1306_DrawText(buf1, Font7x13, 28, 32, White);
	  ssd1306_DrawText(buf2, Font7x13, 28, 43, White);
	  //Bottom buttons
	  const char btnL_lbl[2] = "<";
	  const char buf_row3[] = "Temps";
	  const char btnR_lbl[2] = ">";
	  ssd1306_DrawText(btnL_lbl, Font6x8, 28, 56, White);
	  ssd1306_DrawText(buf_row3, Font6x8, 50, 56, White);
	  ssd1306_DrawText(btnR_lbl, Font6x8, 95, 56, White);
}

static void draw_panel_dateTime(void)
{
		volatile char buff_time[13] = "hh:mm:ss";
		volatile char buff_date[13] = "dd:mm:yy";
		sprintf((char *)buff_time, "%.2d:%.2d:%.2d", RTC_CTRL.hour_c, RTC_CTRL.minutes_c, RTC_CTRL.seconds_c);
		sprintf((char *)buff_date, "%.2d.%.2d.%.2d", RTC_CTRL.day_c, RTC_CTRL.month_c, RTC_CTRL.year_c);
	  const char buf_row3[] = "Time";
	  char buf_row0[] = "Monday";
	  ssd1306_DrawText(buf_row0, Font6x8, 28, 24, White);
//	  SSD1306_DrawFilledRectangle(58, 35, 8, 3, White);
//	  SSD1306_DrawLine(30, 36, 96, 36, White);
	  ssd1306_DrawText(buff_time, Font7x13, 28, 32, White);
	  ssd1306_DrawText(buff_date, Font7x13, 28, 43, White);
	  //Bottom buttons
	  const char btnL_lbl[2] = "<";
//	  char btnMid_lbl[5] = "MENU";
	  const char btnR_lbl[2] = ">";
	  ssd1306_DrawText(btnL_lbl, Font6x8, 28, 56, White);
	  ssd1306_DrawText(buf_row3, Font6x8, 50, 56, White);
	  ssd1306_DrawText(btnR_lbl, Font6x8, 95, 56, White);
}

static void draw_panel_adc(void)
{
	  char buff_adc1[12] = "-19C 2.83V";
	  char buff_adc2[12] = "-19C 2.83V";
	  const char buf_row0[] = "ADC:";
	  ssd1306_DrawText(buf_row0, Font6x8, 45, 24, White);
//	  SSD1306_DrawFilledRectangle(58, 35, 8, 3, White);
//	  SSD1306_DrawLine(30, 36, 96, 36, White);
//	  ssd1306_DrawText(buff_time, Font6x8, 28, 41, White);
	  sprintf((char *)buff_adc1, "%.2d in 6", CTRLdevice.temp_int_raw);
	  sprintf((char *)buff_adc2, "%.2d vbat", CTRLdevice.vBat_raw);

	  ssd1306_DrawText(buff_adc1, Font6x8, 28, 34, White);
	  ssd1306_DrawText(buff_adc2, Font6x8, 28, 43, White);
	  //Bottom buttons
	  const char btnL_lbl[2] = "<";
	  const char btnMid_lbl[5] = "ADCs";
	  const char btnR_lbl[2] = ">";
	  ssd1306_DrawText(btnL_lbl, Font6x8, 28, 56, White);
	  ssd1306_DrawText(btnMid_lbl, Font6x8, 50, 56, White);
	  ssd1306_DrawText(btnR_lbl, Font6x8, 95, 56, White);
}


void ctrl_menu(CTRL_Menu_t *ctrl)
{
	switch (ctrl->menuPos)
	{
	case 1:
		draw_panel_dateTime();
		break;
	case 2:
		draw_panel_adc();
		break;
	default:
		draw_panel_climate();
		break;
	}
}


