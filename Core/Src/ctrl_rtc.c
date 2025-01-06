/*
 * ctrl_rtc.c
 *
 *  Created on: Jan 4, 2025
 *      Author: x280
 */

#include "ctrl_rtc.h"
#include "stdio.h"
#include "main.h"

RTC_t RTC_CTRL={
		.rtcSetFlag= RTC_FLAG_NO_SET,
		.day_s = 27,
		.month_s= 10,
		.year_s= 22,

		.hour_s= 12,
		.minutes_s= 00,
		.seconds_s= 0,

		.dayWeek_s= LL_RTC_WEEKDAY_THURSDAY,
};

void rtc_init(void)
{
	LL_RCC_EnableRTC();
}

void rtc_update(void)
{
	RTC_CTRL.hour_c= 	__LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
	RTC_CTRL.minutes_c= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
	RTC_CTRL.seconds_c= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));

	RTC_CTRL.day_c= 	__LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
	RTC_CTRL.month_c= 	__LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
	RTC_CTRL.year_c= 	__LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));

	RTC_CTRL.dayWeek_c= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));


}

void rtc_allow_set(void)
{
	RTC_CTRL.rtcSetFlag = RTC_FLAG_YES_SET;
}

void rtc_set(void)
{
	if(RTC_CTRL.rtcSetFlag == RTC_FLAG_YES_SET)
	{
		LL_RTC_DisableWriteProtection(RTC);	//enable access to rtc domain regs
		LL_RTC_EnableInitMode(RTC);
		while(LL_RTC_IsActiveFlag_INIT(RTC) != 1)
		{
			#if DEBUG_DEVICE
			printf("\n wait for rtc availability to edit");
			#endif
		}

		LL_RTC_DATE_Config(RTC, __LL_RTC_CONVERT_BIN2BCD(RTC_CTRL.dayWeek_s),
								__LL_RTC_CONVERT_BIN2BCD(RTC_CTRL.day_s),
								__LL_RTC_CONVERT_BIN2BCD(RTC_CTRL.month_s),
								__LL_RTC_CONVERT_BIN2BCD(RTC_CTRL.year_s));
		LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_AM_OR_24,
								__LL_RTC_CONVERT_BIN2BCD(RTC_CTRL.hour_s),
								__LL_RTC_CONVERT_BIN2BCD(RTC_CTRL.minutes_s),
								__LL_RTC_CONVERT_BIN2BCD(RTC_CTRL.seconds_s));

		LL_RTC_DisableInitMode(RTC);
		LL_RTC_ClearFlag_RS(RTC);

		while(LL_RTC_IsActiveFlag_RS(RTC) != 1)
		{
			#if DEBUG_DEVICE
			printf("\n waiting for rtc to sunchronize");
			#endif
		}

		LL_RTC_EnableWriteProtection(RTC);	//locks access to rtc domain regs
		rtc_update();
		RTC_CTRL.rtcSetFlag = RTC_FLAG_NO_SET;
	}
}

