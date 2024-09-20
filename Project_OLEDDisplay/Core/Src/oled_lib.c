/**
  ******************************************************************************
  * @file    oled_lib.c
  * @author  Mathias Bohle
  * @date 	 14.01.2024
  * @brief	 Abstraction Layer for some Commands for interaction with display
  * 		 and to improve Program Layout
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "oled_lib.h"

/* Globals -------------------------------------------------------------------*/
static char write_buffer [30];

/**
  * @brief Draws words "PRESS BUTTON TO CONTINUE..." over loading screen
  * @param None
  * @return None
  */
void oled_continueMessage(void)
{
	oled_FillArea(29, 7, 67, 24, 0xFFFF); //Delete upper part of image
	snprintf( write_buffer, 30, "PRESS BUTTON" );
	oled_writeText( &write_buffer[0], 11, 5 );
	snprintf( write_buffer, 30, "TO CONTINUE..." );
	oled_writeText( &write_buffer[0], 8, 15 );
}
/**
  * @brief Toggles third Button ..[.] of loading screen according to ANIMATED_DOT state
  * @param ANIMATED_DOT ON/OFF
  * @return None
  */
void oled_continueMessageDot(ANIMATED_DOT_t status)
{
	oled_FillArea(85, 22, 86, 24, status);
}
/**
  * @brief Draws menu Layout with 4 different options
  * @param char arrays of the 4 options
  * @return None
  */
void oled_drawMainMenu(const char *item0,const char *item1,const char *item2,const char *item3)
{
	snprintf( write_buffer, 30, "MENU" );
	oled_writeText( &write_buffer[0], 4, 1 );
	oled_FillArea(0, 12, 96, 13, 0x9494);
	oled_FillArea(88, 12, 89, 96, 0x9494);
	oled_FillArea(0, 12, 1, 96, 0x9494);
	oled_FillArea(0, 33, 89, 34, 0x9494);
	oled_FillArea(0, 54, 89, 55, 0x9494);
	oled_FillArea(0, 75, 89, 76, 0x9494);
	oled_FillArea(0, 95, 89, 96, 0x9494);

	snprintf( write_buffer, 30, item0 );
	oled_writeText( &write_buffer[0], 4, 17 );

	snprintf( write_buffer, 30, item1 );
	oled_writeText( &write_buffer[0], 4, 38 );

	snprintf( write_buffer, 30, item2 );
	oled_writeText( &write_buffer[0], 4, 59 );

	snprintf( write_buffer, 30, item3 );
	oled_writeText( &write_buffer[0], 4, 80 );
}
/**
  * @brief Highlights one of the Menu Options without needing to refresh whole screen
  * @param Choosen Menu Item
  * @return None
  */
void oled_highlightMainItem(MENU_ITEM_t item)
{
	if(item == FIRST_ITEM)
	{
		oled_FillArea(1,13,88,14,0x6b6d);
		oled_FillArea(1,32,88,33,0x6b6d);
		oled_FillArea(1,13,2,33,0x6b6d);
		oled_FillArea(87,13,88,33,0x6b6d);
	}
	else
	{
		oled_FillArea(1,13,88,14,0xFFFF);
		oled_FillArea(1,32,88,33,0xFFFF);
		oled_FillArea(1,13,2,33,0xFFFF);
		oled_FillArea(87,13,88,33,0xFFFF);
	}
	if(item == SECOND_ITEM)
	{
		oled_FillArea(1,34,88,35,0x6b6d);
		oled_FillArea(1,53,88,54,0x6b6d);
		oled_FillArea(1,34,2,54,0x6b6d);
		oled_FillArea(87,34,88,54,0x6b6d);
	}
	else
	{
		oled_FillArea(1,34,88,35,0xFFFF);
		oled_FillArea(1,53,88,54,0xFFFF);
		oled_FillArea(1,34,2,54,0xFFFF);
		oled_FillArea(87,34,88,54,0xFFFF);
	}
	if(item == THIRD_ITEM)
	{
		oled_FillArea(1,55,88,56,0x6b6d);
		oled_FillArea(1,74,88,75,0x6b6d);
		oled_FillArea(1,55,2,75,0x6b6d);
		oled_FillArea(87,55,88,75,0x6b6d);
	}
	else
	{
		oled_FillArea(1,55,88,56,0xFFFF);
		oled_FillArea(1,74,88,75,0xFFFF);
		oled_FillArea(1,55,2,75,0xFFFF);
		oled_FillArea(87,55,88,75,0xFFFF);
	}
	if(item == FOURTH_ITEM)
	{
		oled_FillArea(1,76,88,77,0x6b6d);
		oled_FillArea(1,94,88,95,0x6b6d);
		oled_FillArea(1,76,2,95,0x6b6d);
		oled_FillArea(87,76,88,95,0x6b6d);
	}
	else
	{
		oled_FillArea(1,76,88,77,0xFFFF);
		oled_FillArea(1,94,88,95,0xFFFF);
		oled_FillArea(1,76,2,95,0xFFFF);
		oled_FillArea(87,76,88,95,0xFFFF);
	}
	if(item == NO_ITEM)
	{
		;
	}

}
/**
  * @brief Draws submenu layout with name of item and lines
  * @param Name, LeftButton and RightButton Message
  * @return None
  */
void oled_drawItemMenu(const char *name, const char *Left, const char *Right)
{
	oled_blankScreen();
	snprintf( write_buffer, 30, name);
	oled_writeText( &write_buffer[0], 4, 1 );

	oled_FillArea(0, 12, 96, 13, 0x9494);

	oled_FillArea(0, 72, 96, 73, 0x9494);
	oled_FillArea(0, 73, 1, 95, 0x9494);
	oled_FillArea(47, 73, 48, 95, 0x9494);
	oled_FillArea(95, 73, 96, 95, 0x9494);
	oled_FillArea(0, 95, 96, 96, 0x9494);

	snprintf( write_buffer, 30, Left );
	oled_writeText( &write_buffer[0], 7, 79 );
	snprintf( write_buffer, 30, Right );
	oled_writeText( &write_buffer[0], 60, 79 );


}
/**
  * @brief highlights one of the submenu options without refreshing the
  * 	   whole screen.
  * @param submenu item to highlight
  * @return None
  */
void oled_highlightItemLR(SUBMENU_STATE_t item)
{
	if(item == REPEAT)
	{
		  oled_FillArea(1, 73, 46, 74, 0x6b6d);
		  oled_FillArea(1, 94, 46, 95, 0x6b6d);
		  oled_FillArea(1, 73, 2, 95, 0x6b6d);
		  oled_FillArea(46, 73, 47, 95, 0x6b6d);

		  oled_FillArea(48, 73, 95, 74, 0xFFFF);
		  oled_FillArea(48, 94, 95, 95, 0xFFFF);
		  oled_FillArea(48, 73, 49, 95, 0xFFFF);
		  oled_FillArea(94, 73, 95, 95, 0xFFFF);
	}
	else if(item == BACK)
	{
		  oled_FillArea(48, 73, 95, 74, 0x6b6d);
		  oled_FillArea(48, 94, 95, 95, 0x6b6d);
		  oled_FillArea(48, 73, 49, 95, 0x6b6d);
		  oled_FillArea(94, 73, 95, 95, 0x6b6d);

		  oled_FillArea(1, 73, 46, 74, 0xFFFF);
		  oled_FillArea(1, 94, 46, 95, 0xFFFF);
		  oled_FillArea(1, 73, 2, 95, 0xFFFF);
		  oled_FillArea(46, 73, 47, 95, 0xFFFF);
	}
}
/**
  * @brief Turns whole screen back to blank/white
  * @param None
  * @return None
  */
void oled_blankScreen(void)
{
	//TODO all functions that write to screen are slow, DrawBitmap is fast. Better implementation would be to fill things in buffer and then send buffer quickly
	oled_DrawBitmap(&blank_bmp[0], 0, 0);
}
/**
  * @brief Draws loading screen
  * @param None
  * @return None
  */
void oled_loadingScreen(void)
{
	oled_DrawBitmap(&logo_bmp[0], 0, 0); //Show image
}
/**
  * @brief Draws cursor of submenu "SET COLOR" accordingly
  * @param current color and momentary value
  * @return None
  */
void oled_SetColorCursor(SET_COLOR_STATE_t color, uint16_t level)
{
	level = level*60;
	level = level/256;
	if(color == RED)
	{
		oled_FillArea(30, 20, 90, 25, 0xFFFF);
		oled_FillArea(30, 22, 90, 23, 0xF800);
		oled_FillArea(30, 20, 31, 25, 0xF800);
		oled_FillArea(89, 20, 90, 25, 0xF800);
		oled_FillArea(30+level, 20, 31+level, 25, 0);
	}
	else if(color == GREEN)
	{
		oled_FillArea(30, 39, 90, 44, 0xFFFF);
		oled_FillArea(30, 41, 90, 42, 0x07E0);
		oled_FillArea(30, 39, 31, 44, 0x07E0);
		oled_FillArea(89, 39, 90, 44, 0x07E0);
		oled_FillArea(30+level, 39, 31+level, 44, 0);
	}
	else if(color == BLUE)
	{
		oled_FillArea(30, 59, 90, 64, 0xFFFF);
		oled_FillArea(30, 61, 90, 62, 0x001F);
		oled_FillArea(30, 59, 31, 64, 0x001F);
		oled_FillArea(89, 59, 90, 64, 0x001F);
		oled_FillArea(30+level, 59, 31+level, 64, 0);
	}
}

