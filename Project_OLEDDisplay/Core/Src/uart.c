/**
  ******************************************************************************
  * @file    uart.c
  * @author  Mathias Bohle
  * @date 	 29.04.2024
  * @brief   Controls uart hardware.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "stdbool.h"
#include "tasks.h"

/* Globals -------------------------------------------------------------------*/
osMessageQueueId_t UartUpdateQueueHandle;

const osMessageQueueAttr_t UartUpdateQueue_attributes = {
  .name = "UartUpdateQueue"
};

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
	UartUpdateQueueHandle = osMessageQueueNew(2, sizeof(struct MEASUREMENT_S), &UartUpdateQueue_attributes);
	if(UartUpdateQueueHandle == NULL)
		 return UART_ERROR;

	//register own callback function (needs a setting in .ioc file to work)
	HAL_UART_RegisterRxEventCallback(&huart1,uart_callback);
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)RxData, RX_BUFFER_SIZE);
	return UART_CREATED;
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
		struct MEASUREMENT_S CurrentValues;
		CurrentValues.red = 0;
		CurrentValues.green = 0;
		CurrentValues.blue = 0;
		CurrentValues.clear = 0;
		CurrentValues.infrared = 0;
		int r,g,b,c,ir;
		sscanf(RxData, "MEA:%u,%u,%u,%u,%u\r\n", &r, &g, &b, &ir, &c);
		CurrentValues.red = r;
		CurrentValues.green = g;
		CurrentValues.blue = b;
		CurrentValues.clear = c;
		CurrentValues.infrared = ir;
		osMessageQueuePut(UartUpdateQueueHandle, &CurrentValues, 0, 0);
		osEventFlagsSet(colorUpdateEventHandle,MEASUREMENT_DONE);

	}
	//Restart receive to idle
	if(check_for_buffer_overflow(&huart1))
	{
		printf("MEA:\r\n");
		clear_buffer_overflow(&huart1);
	}
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t*)RxData,RX_BUFFER_SIZE);


}
