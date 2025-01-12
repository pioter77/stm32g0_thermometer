/*
 * ctrl_menu.h
 *
 *  Created on: Jan 6, 2025
 *      Author: x280
 */

#ifndef INC_CTRL_MENU_H_
#define INC_CTRL_MENU_H_

#include "main.h"


typedef struct CTRL_Menu_T{
	uint8_t menuPos;
	uint8_t menuMax;

	_Bool drawMainPanels;

	volatile _Bool btnLpressed;
	volatile _Bool btnMpressed;
	volatile _Bool btnRpressed;
}CTRL_Menu_t;

extern CTRL_Menu_t CTRLmenu;

void ctrl_menu(CTRL_Menu_t *ctrl);



#endif /* INC_CTRL_MENU_H_ */
