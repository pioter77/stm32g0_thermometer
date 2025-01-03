#ifndef FONTS_H
#define FONTS_H 120

#include "stm32g0xx.h"
#include "string.h"
#include <stdint.h>


#define FONT_6_X_8
//#define FONT_7_X_9
//#define FONT_7_X_10
//#define FONT_7_X_10
//#define FONT_11_X_18
//#define FONT_16_X_26


typedef struct {
    uint8_t FontWidth;
    uint8_t FontHeight;
    const uint16_t *data;   //ptr to data font array
} FontDef_t;

typedef struct {
    uint16_t Length;
    uint16_t Height;
} FONTS_SIZE_t;


#ifdef FONT_6_X_8
	extern const uint8_t Font6x8 [];
	//extern FontDef_t Font_6x8;
#endif

#ifdef FONT_7_X_9
	extern FontDef_t Font_7x9;
#endif

#ifdef FONT_7_X_10
	extern FontDef_t Font_7x10;
#endif

#ifdef FONT_11_X_18
	extern FontDef_t Font_11x18;
#endif

#ifdef FONT_16_X_26
	extern FontDef_t Font_16x26;
#endif

//extern const uint16_t Font7x10[];
//extern const uint16_t Font11x18[];
//extern const uint16_t Font16x26[];



//for calculating string len an height in pixels based on string length and its font
char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);

#endif
