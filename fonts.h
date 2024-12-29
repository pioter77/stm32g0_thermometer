#ifndef FONTS_H
#define FONTS_H 120

#include "stm32g0xx.h"
#include "string.h"
#include <stdint.h>

#endif

typedef struct {
    uint8_t FontWidth;
    uint8_t FontHeight;
    const uint16_t *data;   //ptr to data font array
} FontDef_t;

typedef struct {
    uint16_t Length;
    uint16_t Height;
} FONTS_SIZE_t;

extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;

//for calculating string len an height in pixels based on string length and its font
char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);
