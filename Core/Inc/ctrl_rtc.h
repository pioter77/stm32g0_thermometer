/*
 * ctrl_rtc.h
 *
 *  Created on: Jan 4, 2025
 *      Author: x280
 */

#ifndef INC_CTRL_RTC_H_
#define INC_CTRL_RTC_H_

#include "main.h"
#include "rtc.h"


typedef enum {RTC_FLAG_NO_SET=0, RTC_FLAG_YES_SET=1} rtcSetFlag_t;

typedef struct rtc_T{
	//curent date/time:
	uint8_t day_c;
	uint8_t month_c;
	uint8_t year_c;

	uint8_t hour_c;
	uint8_t minutes_c;
	uint8_t seconds_c;

	uint8_t dayWeek_c;



	// date/time to  update registers with and start counting from new values:
	uint8_t day_s;
	uint8_t month_s;
	uint8_t year_s;

	uint8_t hour_s;
	uint8_t minutes_s;
	uint8_t seconds_s;

	uint8_t dayWeek_s;


	rtcSetFlag_t rtcSetFlag;		//if flag to yes then we update registers with new walue of date/time
}RTC_t;

extern RTC_t RTC_CTRL;

void rtc_init(void);
void rtc_allow_set(void);
void rtc_update(void);
void rtc_set(void);

#endif /* INC_CTRL_RTC_H_ */
