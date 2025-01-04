#ifndef FONTS_H
#define FONTS_H 120

#include "stm32g0xx.h"
#include "string.h"
#include <stdint.h>


#define FONT_6_X_8
#define FONT_7_X_13


#ifdef FONT_6_X_8
	extern const uint8_t Font6x8 [];
	//extern FontDef_t Font_6x8;
#endif

#ifdef FONT_7_X_13
	extern const uint8_t Font7x13 [];
#endif

//extern const uint16_t Font16x26[];



//for calculating string len an height in pixels based on string length and its font
//char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);

#endif
