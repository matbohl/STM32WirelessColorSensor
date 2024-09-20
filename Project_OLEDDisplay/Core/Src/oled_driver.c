/**
  ******************************************************************************
  * @file    oled_driver.c
  * @author  Mathias Bohle
  * @date 	 07.01.2024
  * @brief	 Handles SPI Communication with OLED C Click, also provides
  * 		 functions to configure and draw on display
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "oled_driver.h"

/* Private Function Prototypes -----------------------------------------------*/
static void pixel( uint8_t col, uint8_t row, uint16_t color );
void character( uint16_t ch );
void draw_area( uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, const uint8_t *img );


/* Defines -------------------------------------------------------------------*/

//Font Direction
const uint8_t  OLED_FONT_HORIZONTAL       = 0x00;
const uint8_t  OLED_FONT_VERTICAL         = 0x01;
const uint8_t  OLED_FONT_VERTICAL_COLUMN  = 0x02;

//Re-Map Settings
const uint8_t OLED_RMP_INC_HOR         = 0x00;
const uint8_t OLED_RMP_INC_VER         = 0x01;
const uint8_t OLED_RMP_COLOR_NOR       = 0x00;
const uint8_t OLED_RMP_COLOR_REV       = 0x02;
const uint8_t OLED_RMP_SEQ_RGB         = 0x00;
const uint8_t OLED_RMP_SEQ_BGR         = 0x04;
const uint8_t OLED_RMP_SCAN_NOR        = 0x00;
const uint8_t OLED_RMP_SCAN_REV        = 0x10;
const uint8_t OLED_RMP_SPLIT_DISABLE   = 0x00;
const uint8_t OLED_RMP_SPLIT_ENABLE    = 0x20;
const uint8_t OLED_COLOR_65K           = 0x00;
const uint8_t OLED_COLOR_262K          = 0x80;
const uint8_t OLED_IMG_HEAD            = 0x06;

//Device Properties
const uint8_t  OLED_SCREEN_WIDTH    = 0x60;
const uint8_t  OLED_SCREEN_HEIGHT   = 0x60;
const uint16_t OLED_SCREEN_SIZE     = 0x2400;
const uint8_t  OLED_ROW_OFF         = 0x00;
const uint8_t  OLED_COL_OFF         = 0x10;

//Controller Commands
const uint8_t  OLED_SET_COL_ADDRESS   = 0x15;
const uint8_t  OLED_SET_ROW_ADDRESS   = 0x75;
const uint8_t  OLED_WRITE_RAM         = 0x5C;
const uint8_t  OLED_READ_RAM          = 0x5D;
const uint8_t  OLED_SET_REMAP         = 0xA0;
const uint8_t  OLED_SET_START_LINE    = 0xA1;
const uint8_t  OLED_SET_OFFSET        = 0xA2;
const uint8_t  OLED_MODE_OFF          = 0xA4;
const uint8_t  OLED_MODE_ON           = 0xA5;
const uint8_t  OLED_MODE_NORMAL       = 0xA6;
const uint8_t  OLED_MODE_INVERSE      = 0xA7;
const uint8_t  OLED_FUNCTION          = 0xAB;
const uint8_t  OLED_SLEEP_ON          = 0xAE;
const uint8_t  OLED_SLEEP_OFF         = 0xAF;
const uint8_t  OLED_NOP               = 0xB0;
const uint8_t  OLED_SET_RESET_PRECH   = 0xB1;
const uint8_t  OLED_ENHANCEMENT       = 0xB2;
const uint8_t  OLED_CLOCK_DIV         = 0xB3;
const uint8_t  OLED_VSL               = 0xB4;
const uint8_t  OLED_GPIO              = 0xB5;
const uint8_t  OLED_SETSEC_PRECH      = 0xB6;
const uint8_t  OLED_GREY_SCALE        = 0xB8;
const uint8_t  OLED_LUT               = 0xB9;
const uint8_t  OLED_PRECH_VOL         = 0xBB;
const uint8_t  OLED_VCOMH             = 0xBE;
const uint8_t  OLED_CONTRAST          = 0xC1;
const uint8_t  OLED_MASTER_CONTRAST   = 0xC7;
const uint8_t  OLED_MUX_RATIO         = 0xCA;
const uint8_t  OLED_COMMAND_LOCK      = 0xFD;
const uint8_t  OLED_SCROLL_HOR        = 0x96;
const uint8_t  OLED_START_MOV         = 0x9E;
const uint8_t  OLED_STOP_MOV          = 0x9F;

/* Globals -------------------------------------------------------------------*/
SPI_HandleTypeDef* hspi_local = NULL;

static uint8_t OLED_DEFAULT_MUX_RATIO      = 95;
static uint8_t OLED_DEFAULT_START_LINE     = 0x80;
static uint8_t OLED_DEFAULT_OFFSET         = 0x20;

static uint8_t OLED_DEFAULT_OLED_LOCK      = 0x12;
static uint8_t OLED_DEFAULT_CMD_LOCK       = 0xB1;
static uint8_t OLED_DEFAULT_DIVSET         = 0xF1;
static uint8_t OLED_DEFAULT_PRECHARGE      = 0x32;
static uint8_t OLED_DEFAULT_VCOMH          = 0x05;
static uint8_t OLED_DEFAULT_MASTER_CONT    = 0x0E; //100 cd/m^2 from data sheet of display
static uint8_t OLED_DEFAULT_PRECHARGE_2    = 0x01;

static  uint8_t OLED_DEFAULT_VSL[ 3 ]       = { 0xA0, 0xB5, 0x55 };
static  uint8_t OLED_DEFAULT_CONTRAST[ 3 ]  = { 0x75, 0x42, 0x49 }; //100 cd/m^2 from data sheet of display
static  uint8_t OLED_ENCHANCED[3]           = { 0xA4, 0x00, 0x00 }; //Magic enchancement?!

static const uint8_t*   _font;
static uint16_t         _font_color;
static uint8_t          _font_orientation;
static uint16_t         _font_first_char;
static uint16_t         _font_last_char;
static uint16_t         _font_height;
static uint16_t         x_cord;
static uint16_t         y_cord;

static uint8_t cols[ 2 ]    = { OLED_COL_OFF, OLED_COL_OFF + 95 };
static uint8_t rows[ 2 ]    = { OLED_ROW_OFF, OLED_ROW_OFF + 95 };

static uint8_t OLED_DEFAULT_REMAP = OLED_RMP_INC_HOR | OLED_RMP_COLOR_REV |
                                OLED_RMP_SEQ_RGB | OLED_RMP_SCAN_REV |
                                OLED_RMP_SPLIT_ENABLE | OLED_COLOR_65K;

/* Functions -----------------------------------------------------------------*/
/**
  * @brief Initiates the OLED Display with black background and sets up Font.
  * @param SPI_HandleTypeDef Copy of spi handle from main()
  * @return None
  */
void oled_Init(SPI_HandleTypeDef* hspi)
{
	//copy SPI Handle
	hspi_local = hspi;

	//Unlock display
	oled_SendCommand( OLED_COMMAND_LOCK,    &OLED_DEFAULT_OLED_LOCK,     1 );
	oled_SendCommand( OLED_COMMAND_LOCK,    &OLED_DEFAULT_CMD_LOCK,      1 );

	//Controller Setup
	oled_SendCommand( OLED_SET_REMAP,       &OLED_DEFAULT_REMAP,         1 );
	oled_SendCommand( OLED_MUX_RATIO,       &OLED_DEFAULT_MUX_RATIO,     1 );
	oled_SendCommand( OLED_SET_START_LINE,  &OLED_DEFAULT_START_LINE,    1 );
	oled_SendCommand( OLED_SET_OFFSET,      &OLED_DEFAULT_OFFSET,        1 );
	oled_SendCommand( OLED_VCOMH,           &OLED_DEFAULT_VCOMH,         1 );
	oled_SendCommand( OLED_CLOCK_DIV,       &OLED_DEFAULT_DIVSET,        1 );
	oled_SendCommand( OLED_SET_RESET_PRECH, &OLED_DEFAULT_PRECHARGE,     1 );
	oled_SendCommand( OLED_SETSEC_PRECH,    &OLED_DEFAULT_PRECHARGE_2,   1 );
	oled_SendCommand( OLED_MASTER_CONTRAST, &OLED_DEFAULT_MASTER_CONT,   1 );
	oled_SendCommand( OLED_CONTRAST,        OLED_DEFAULT_CONTRAST,       3 );
	oled_SendCommand( OLED_VSL,             OLED_DEFAULT_VSL,            3 );
	oled_SendCommand( OLED_ENHANCEMENT,     OLED_ENCHANCED,              3 );

	//Mode Normal
	oled_SendCommand( OLED_MODE_NORMAL, 0, 0 );

	//Turn on display
	oled_SendCommand( OLED_SLEEP_OFF,   0, 0 );

	//Blank screen
	oled_FillScreen(0);

	oled_setFont(&guiFont_Tahoma_7_Regular[0], 0, OLED_FONT_HORIZONTAL);
}
/* Functions -----------------------------------------------------------------*/
/**
  * @brief Sends Command to Display over SPI, also handles Chipselect and
  * 	   Command Pin
  * @param uint8_t command, 8Bit or 2xBit data , uint16_t length of data
  * @return None
  */
void oled_SendCommand( uint8_t command, uint8_t *args, uint16_t args_len )
{
    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_COMMAND);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, SELECTED);

    HAL_SPI_Transmit(hspi_local, &command, 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_DATA);

    if(args_len)
    	HAL_SPI_Transmit(hspi_local, args, args_len, 100);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, NOT_SELECTED);
}
/**
  * @brief Fills square Area between start coordinates and end coordinates (each in row/column)
  * 	   with color.
  * @param uint8_t start end end coordinates, uint16_t color
  * @return None
  */
void oled_FillArea( uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, uint16_t color )
{
	//TODO oled_FillArea often used to draw a line -> implement "oled_DrawLine()" function
    uint8_t   cmd       = OLED_WRITE_RAM;
    uint16_t  cnt       = ( end_col - start_col ) * ( end_row - start_row );
    uint8_t   clr[ 2 ]  = { 0 };

    if( ( start_col > OLED_SCREEN_WIDTH ) ||
        ( end_col > OLED_SCREEN_WIDTH ) )
        return;

    if( ( start_row > OLED_SCREEN_HEIGHT ) ||
        ( end_row > OLED_SCREEN_HEIGHT ) )
        return;

    if( ( end_col < start_col ) ||
        ( end_row < start_row ) )
        return;

    cols[ 0 ] = OLED_COL_OFF + start_col;
    cols[ 1 ] = OLED_COL_OFF + end_col - 1;
    rows[ 0 ] = OLED_ROW_OFF + start_row;
    rows[ 1 ] = OLED_ROW_OFF + end_row - 1;
    clr[ 0 ] |= color >> 8;
    clr[ 1 ] |= color & 0x00FF;

    oled_SendCommand( OLED_SET_COL_ADDRESS, cols, 2 );
    oled_SendCommand( OLED_SET_ROW_ADDRESS, rows, 2 );

    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_COMMAND);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, SELECTED);

    HAL_SPI_Transmit(hspi_local, &cmd, 1, 100);

    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_DATA);

    while(cnt--)
    	HAL_SPI_Transmit(hspi_local, clr, 2, 100);

    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, NOT_SELECTED);
}
/**
  * @brief	Fills complete Screen with color
  * @param uint16_t color
  * @return None
  */
void oled_FillScreen( uint16_t color )
{
    oled_FillArea( 0, 0, OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, color );
}
/**
  * @brief Draws bitmap array on screen
  * @param bitmap array and start coordinates
  * @return None
  */
void oled_DrawBitmap(const uint8_t* img, uint8_t col_off, uint8_t row_off )
{
    const uint8_t *ptr = img;
    draw_area( col_off, row_off, col_off + ptr[2], row_off + ptr[4], ptr );
}
/**
  * @brief copys important details from font
  * @param font array, color of text and orientation
  * @return None
  */
void oled_setFont( const uint8_t *font, uint16_t color, uint8_t orientation )
{
    _font               = font;
    _font_first_char    = font[2] + (font[3] << 8);
    _font_last_char     = font[4] + (font[5] << 8);
    _font_height        = font[6];
    _font_color         = color;
    _font_orientation   = orientation ;
}
/**
  * @brief Writes text from char array
  * @param char array, start coordinates
  * @return None
  */
void oled_writeText( char *text, uint16_t x, uint16_t y )
{
	char *ptr = text;

    if( ( x >= OLED_SCREEN_WIDTH ) || (y >= OLED_SCREEN_HEIGHT ) )
        return;

    x_cord = x;
    y_cord = y;

    while(*ptr)
    {
    	character( *ptr++ );
    }
}

/* Private Functions ---------------------------------------------------------*/
/**
  * @brief Helperfunction for writeText
  * @param single character
  * @return None
  */
void character( uint16_t ch )
{
    uint8_t     ch_width = 0;
    uint8_t     x_cnt;
    uint8_t     y_cnt;
    uint16_t    x = 0;
    uint16_t    y = 0;
    uint16_t    tmp;
    uint8_t     temp = 0;
    uint8_t     mask = 0;
    uint32_t    offset;
    const uint8_t *ch_table;
    const uint8_t *ch_bitmap;

    if( ch < _font_first_char )
        return;

    if( ch > _font_last_char )
        return;

    offset = 0;
    tmp = (ch - _font_first_char) << 2;
    ch_table = _font + 8 + tmp;
    ch_width = *ch_table;

    offset = (uint32_t)ch_table[1] + ((uint32_t)ch_table[2] << 8) + ((uint32_t)ch_table[3] << 16);

    ch_bitmap = _font + offset;

    if( ( _font_orientation == OLED_FONT_HORIZONTAL ) ||
        ( _font_orientation == OLED_FONT_VERTICAL_COLUMN ) )
    {
        y = y_cord;
        for (y_cnt = 0; y_cnt < _font_height; y_cnt++)
        {
            x = x_cord;
            mask = 0;
            for( x_cnt = 0; x_cnt < ch_width; x_cnt++ )
            {
                if( !mask )
                {
                    temp = *ch_bitmap++;
                    mask = 0x01;
                }

                if( temp & mask )
                    pixel( x, y, _font_color );

                x++;
                mask <<= 1;
            }
            y++;
        }

        if ( _font_orientation == OLED_FONT_HORIZONTAL )
            x_cord = x + 1;
        else
            y_cord = y;
    }
    else
    {
        y = x_cord;

        for( y_cnt = 0; y_cnt < _font_height; y_cnt++ )
        {
            x = y_cord;
            mask = 0;

            for( x_cnt = 0; x_cnt < ch_width; x_cnt++ )
            {
                if( mask == 0 )
                {
                    temp = *ch_bitmap++;
                    mask = 0x01;
                }

                if( temp & mask )
                {
                    pixel( y, x, _font_color );
                }

                x--;
                mask <<= 1;
            }
            y++;
        }
        y_cord = x - 1;
    }
}

/**
  * @brief Helperfunction for draw bitmap
  * @param start and end coordinates, pointer to bitmap array
  * @return None
  */
void draw_area( uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, const uint8_t *img )
{
    uint8_t     cmd  = OLED_WRITE_RAM;
    uint8_t*  ptr = (uint8_t*)img + OLED_IMG_HEAD;

    if( ( start_col > OLED_SCREEN_WIDTH ) ||
        ( end_col > OLED_SCREEN_WIDTH ) )
        return;

    if( ( start_row > OLED_SCREEN_HEIGHT ) ||
        ( end_row > OLED_SCREEN_HEIGHT ) )
        return;

    if( ( end_col < start_col ) ||
        ( end_row < start_row ) )
        return;

    cols[ 0 ] = OLED_COL_OFF + start_col;
    cols[ 1 ] = OLED_COL_OFF + end_col - 1;
    rows[ 0 ] = OLED_ROW_OFF + start_row;
    rows[ 1 ] = OLED_ROW_OFF + end_row - 1;

    oled_SendCommand( OLED_SET_COL_ADDRESS, cols, 2 );
    oled_SendCommand( OLED_SET_ROW_ADDRESS, rows, 2 );


    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_COMMAND);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, SELECTED);
    HAL_SPI_Transmit(hspi_local, &cmd, 1, 100);
    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_DATA);

    HAL_SPI_Transmit(hspi_local, ptr, 18432, 100);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, NOT_SELECTED);
}
/**
  * @brief Helper function for draw_character, draws single pixels
  * @param coordinates and color of pixel
  * @return None
  */
void pixel( uint8_t col, uint8_t row, uint16_t color )
{
    uint8_t cmd       = OLED_WRITE_RAM;
    uint8_t clr[ 2 ]  = { 0 };

    if( ( col > OLED_SCREEN_WIDTH ) || ( row > OLED_SCREEN_HEIGHT ) )
        return;

    cols[ 0 ] = OLED_COL_OFF + col;
    cols[ 1 ] = OLED_COL_OFF + col;
    rows[ 0 ] = OLED_ROW_OFF + row;
    rows[ 1 ] = OLED_ROW_OFF + row;
    clr[ 0 ] |= color >> 8;
    clr[ 1 ] |= color & 0x00FF;

    oled_SendCommand( OLED_SET_COL_ADDRESS, cols, 2 );
    oled_SendCommand( OLED_SET_ROW_ADDRESS, rows, 2 );

    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_COMMAND);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, SELECTED);

    HAL_SPI_Transmit(hspi_local, &cmd, 1, 1);

    HAL_GPIO_WritePin(OLED_CMD_DATA_GPIO_Port, OLED_CMD_DATA_Pin, SEND_DATA);

    HAL_SPI_Transmit(hspi_local, clr, 2, 1);

    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, NOT_SELECTED);
}
