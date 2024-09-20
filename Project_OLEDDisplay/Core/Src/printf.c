/**
  ******************************************************************************
  * @file    printf.c
  * @author  Mathias Bohle
  * @date 	 29.04.2024
  * @brief   Provides traditional printf() functionality
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "printf.h"

/* prototype -----------------------------------------------------------------*/
PUTCHAR_PROTOTYPE {
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
