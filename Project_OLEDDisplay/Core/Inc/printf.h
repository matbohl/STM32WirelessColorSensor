/**
  ******************************************************************************
  * @file    printf.h
  * @author  Mathias Bohle
  * @date 	 29.04.2024
  * @brief   Provides traditional printf() functionality
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_PRINTF_H_
#define INC_PRINTF_H_

/*Type Definitions -----------------------------------------------------------*/
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

/* Globals -------------------------------------------------------------------*/
extern UART_HandleTypeDef huart1;

#endif /* INC_PRINTF_H_ */
