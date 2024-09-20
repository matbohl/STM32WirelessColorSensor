/**
  ******************************************************************************
  * @file    uart.h
  * @author  Mathias Bohle
  * @date 	 29.04.2024
  * @brief   Controls uart hardware.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_UART_H_
#define INC_UART_H_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "stdio.h"
#include "cmsis_os.h"
/* Globals -------------------------------------------------------------------*/
extern osMessageQueueId_t UartUpdateQueueHandle;

extern const osMessageQueueAttr_t UartUpdateQueue_attributes;

extern UART_HandleTypeDef huart1;
/* Defines -------------------------------------------------------------------*/
typedef enum {
	UART_CREATED = 0,
	UART_ERROR = 1,
}UART_CREATION_t;

#define RX_BUFFER_SIZE 30

/* Function Prototypes -------------------------------------------------------*/
UART_CREATION_t init_uart(void);
void uart_callback(UART_HandleTypeDef *huart, uint16_t size);

#endif /* INC_UART_H_ */
