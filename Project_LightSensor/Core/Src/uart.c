/**
  ******************************************************************************
  * @file    uart.c
  * @author  Mathias Bohle
  * @date 	 29.04.2024
  * @brief   Controls uart hardware
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "stdbool.h"
#include "tasks.h"
#include "pwm_driver.h"

/* Globals -------------------------------------------------------------------*/

char RxData[RX_BUFFER_SIZE];
/* Private Functions ---------------------------------------------------------*/
static _Bool check_for_buffer_overflow(UART_HandleTypeDef *huart)
{
	return __HAL_UART_GET_FLAG(huart,UART_FLAG_ORE);
}
static void clear_buffer_overflow(UART_HandleTypeDef *huart)
{
	__HAL_UART_CLEAR_OREFLAG(huart);
	//Discard Data
	__HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);
}

/* Functions -----------------------------------------------------------------*/
/**
 *  @brief Initiates uart message queue + receive to idle
 *  @param None
 *  @return UART_CREATION_t to make sure queue was created, check for UART_ERROR
 */
UART_CREATION_t init_uart(void)
{
	//register own callback function (needs a setting in .ioc file to work)
	HAL_UART_RegisterRxEventCallback(&huart1,uart_callback);
	if(HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)RxData, RX_BUFFER_SIZE)==HAL_OK)
		return UART_CREATED;
	else
		return UART_ERROR;
}
/* Functions -----------------------------------------------------------------*/
/**
 *  @brief Callback of UART Rx Complete
 *  @param UART_HandleTypeDef *huart, uint16_t size
 *  @return None
 */
void uart_callback(UART_HandleTypeDef *huart, uint16_t size)
{
	if(RxData[0]=='M'&&RxData[1]=='E'&&RxData[2]=='A'&&RxData[3]==':')
	{
		osEventFlagsSet(colorUpdateEventHandle,MEASUREMENT_NEEDED);
	}
	else if(RxData[0]=='C' &&RxData[1]=='O'&& RxData[2]=='L'&& RxData[3]==':')
	{
		RGB_t values;
		values.red = 0;
		values.green = 0;
		values.blue = 0;
		int r,g,b;
		sscanf(RxData, "COL:%i,%i,%i\r\n", &r, &g, &b);
		values.red = r;
		values.green = g;
		values.blue = b;
		osMessageQueuePut(colorUpdateQueueHandle, &values, 0, 0);
		osEventFlagsSet(colorUpdateEventHandle,NEW_COLOR);
	}
	//Restart receive to idle
	if(check_for_buffer_overflow(&huart1))
	{
		printf("ERR:\r\n");
		clear_buffer_overflow(&huart1);
	}
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t*)RxData,RX_BUFFER_SIZE);


}
