/**
  ******************************************************************************
  * @file    pwm_driver.h
  * @author  Mathias Bohle
  * @date 	 05.12.2023
  * @brief   Controls PWM generation for controlling WS2812 RGB LED
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_PWM_DRIVER_H_
#define INC_PWM_DRIVER_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
/* Private typedef -----------------------------------------------------------*/
typedef enum {
	STOPPED = 0,
	RUNNING = 1,
	PROBLEM = 2
} PWM_STATE_t;

typedef struct RGB_Color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}RGB_t;

/* Globals -------------------------------------------------------------------*/

/* Function Prototypes -------------------------------------------------------*/
void pwm_Init(TIM_HandleTypeDef* htim, DMA_HandleTypeDef* hdma);
PWM_STATE_t pwm_SendValues(RGB_t colors);
PWM_STATE_t pwm_StartUpAnimation(void);

#endif /* INC_PWM_DRIVER_H_ */
