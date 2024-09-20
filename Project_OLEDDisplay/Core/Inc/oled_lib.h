/**
  ******************************************************************************
  * @file    oled_lib.h
  * @author  Mathias Bohle
  * @date 	 14.01.2024
  * @brief	 Abstraction Layer for some Commands for interaction with display
  * 		 and to improve Program Layout
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_OLED_LIB_H_
#define INC_OLED_LIB_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"
#include "stdio.h"
#include "oled_driver.h"
#include "bitmaps.h"

/*Type Definitions -----------------------------------------------------------*/
typedef enum {
	NO_ITEM = 0,
	FIRST_ITEM = 1,
	SECOND_ITEM = 2,
	THIRD_ITEM = 3,
	FOURTH_ITEM = 4
}MENU_ITEM_t;

typedef enum {
	DOT_ON = 0,
	DOT_OFF = 0xFFFF
}ANIMATED_DOT_t;

typedef enum {
	WAITING = 0,
	MAIN = 1,
	SUB = 2
}MENU_STATE_t;

typedef enum {
	NONE = 0,
	REPEAT = 1,
	BACK = 2,
	SET_COLOR = 3
}SUBMENU_STATE_t;

typedef enum {
	AGAIN_BACK = 0,
	RED = 1,
	GREEN = 2,
	BLUE = 3

}SET_COLOR_STATE_t;

/* Function Prototypes -------------------------------------------------------*/
void oled_blankScreen(void);
void oled_loadingScreen(void);
void oled_continueMessage(void);
void oled_continueMessageDot(ANIMATED_DOT_t);
void oled_drawMainMenu(const char *item0,const char *item1,const char *item2,const char *item3);
void oled_highlightMainItem(MENU_ITEM_t);
void oled_drawItemMenu(const char *name, const char *Left, const char *Right);
void oled_highlightItemLR(SUBMENU_STATE_t);
void oled_SetColorCursor(SET_COLOR_STATE_t, uint16_t);


#endif /* INC_OLED_LIB_H_ */
