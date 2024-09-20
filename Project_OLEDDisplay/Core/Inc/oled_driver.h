/**
  ******************************************************************************
  * @file    oled_driver.h
  * @author  Mathias Bohle
  * @date 	 07.01.2024
  * @brief	 Handles SPI Communication with OLED C Click, also provides
  * 		 functions to configure and draw on display
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_OLED_DRIVER_H_
#define INC_OLED_DRIVER_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include "single_font.h"


/*Type Definitions -----------------------------------------------------------*/
typedef enum {
	SEND_COMMAND = 0,
	SEND_DATA = 1,
}CMD_DATA_t;

typedef enum {
	SELECTED = 0,
	NOT_SELECTED = 1
}CHIPSELECT_t;

typedef struct RGB_Color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}RGB_t;

/* Defines -------------------------------------------------------------------*/

//Font Direction
extern const uint8_t  OLED_FONT_HORIZONTAL;
extern const uint8_t  OLED_FONT_VERTICAL;
extern const uint8_t  OLED_FONT_VERTICAL_COLUMN;

//Re-Map Settings
extern const uint8_t OLED_RMP_INC_HOR;
extern const uint8_t OLED_RMP_INC_VER;
extern const uint8_t OLED_RMP_COLOR_NOR;
extern const uint8_t OLED_RMP_COLOR_REV;
extern const uint8_t OLED_RMP_SEQ_RGB;
extern const uint8_t OLED_RMP_SEQ_BGR;
extern const uint8_t OLED_RMP_SCAN_NOR;
extern const uint8_t OLED_RMP_SCAN_REV;
extern const uint8_t OLED_RMP_SPLIT_DISABLE;
extern const uint8_t OLED_RMP_SPLIT_ENABLE;
extern const uint8_t OLED_COLOR_65K;
extern const uint8_t OLED_COLOR_262K;
extern const uint8_t OLED_IMG_HEAD;

//Device Properties
extern const uint8_t  OLED_SCREEN_WIDTH;
extern const uint8_t  OLED_SCREEN_HEIGHT;
extern const uint16_t OLED_SCREEN_SIZE;
extern const uint8_t  OLED_ROW_OFF;
extern const uint8_t  OLED_COL_OFF;

//Controller Commands
extern const uint8_t  OLED_SET_COL_ADDRESS;
extern const uint8_t  OLED_SET_ROW_ADDRESS;
extern const uint8_t  OLED_WRITE_RAM;
extern const uint8_t  OLED_READ_RAM;
extern const uint8_t  OLED_SET_REMAP;
extern const uint8_t  OLED_SET_START_LINE;
extern const uint8_t  OLED_SET_OFFSET;
extern const uint8_t  OLED_MODE_OFF;
extern const uint8_t  OLED_MODE_ON;
extern const uint8_t  OLED_MODE_NORMAL;
extern const uint8_t  OLED_MODE_INVERSE;
extern const uint8_t  OLED_FUNCTION;
extern const uint8_t  OLED_SLEEP_ON;
extern const uint8_t  OLED_SLEEP_OFF;
extern const uint8_t  OLED_NOP;
extern const uint8_t  OLED_SET_RESET_PRECH;
extern const uint8_t  OLED_ENHANCEMENT;
extern const uint8_t  OLED_CLOCK_DIV;
extern const uint8_t  OLED_VSL;
extern const uint8_t  OLED_GPIO;
extern const uint8_t  OLED_SETSEC_PRECH;
extern const uint8_t  OLED_GREY_SCALE;
extern const uint8_t  OLED_LUT;
extern const uint8_t  OLED_PRECH_VOL;
extern const uint8_t  OLED_VCOMH;
extern const uint8_t  OLED_CONTRAST;
extern const uint8_t  OLED_MASTER_CONTRAST;
extern const uint8_t  OLED_MUX_RATIO;
extern const uint8_t  OLED_COMMAND_LOCK;
extern const uint8_t  OLED_SCROLL_HOR;
extern const uint8_t  OLED_START_MOV;
extern const uint8_t  OLED_STOP_MOV;

/* Function Prototypes -------------------------------------------------------*/
void oled_Init(SPI_HandleTypeDef* hspi);
void oled_SendCommand( uint8_t command, uint8_t *args, uint16_t args_len );
void oled_FillArea( uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, uint16_t color );
void oled_FillScreen( uint16_t color );
void oled_DrawBitmap(const uint8_t* img, uint8_t col_off, uint8_t row_off );
void oled_setFont( const uint8_t *font, uint16_t color, uint8_t orientation );
void oled_writeText( char *text, uint16_t x, uint16_t y );

#endif /* INC_OLED_DRIVER_H_ */
