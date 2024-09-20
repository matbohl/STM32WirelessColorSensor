/**
  ******************************************************************************
  * @file    pwm_driver.c
  * @author  Mathias Bohle
  * @date 	 05.12.2023
  * @brief   Controls PWM generation for controlling WS2812 RGB LED
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <pwm_driver.h>

/* Globals -------------------------------------------------------------------*/
static _Bool isInitiated = false;
static volatile _Bool isRunning = false;

static TIM_HandleTypeDef* htim_local = NULL;
static DMA_HandleTypeDef* hdma_local = NULL;

static uint32_t pwmData[(240+24+240)]; //delay + color values + delay

/* Functions -----------------------------------------------------------------*/
/**
  * @brief Gets TIM_HandleTypeDef and DMA_HandleTypeDef to start dma later
  * @param TIM_HandleTypeDef* htim, DMA_HandleTypeDef* hdma
  * @return None
  */
void pwm_Init(TIM_HandleTypeDef* htim, DMA_HandleTypeDef* hdma)
{
	//TIM1, DMA, PSC 1, Counter Period 40 (0-39),
	//DMA1, Memory to Peripheral, Normal, Half Word -> Half Word, Increment Memory Adress, TIM2 Channel 1
	//Priority Low
	htim_local = htim;
	hdma_local = hdma;
	isInitiated = true;
}
/**
  * @brief Sends values for single WS2812 RGB LED
  * @param RGB colors (with brightness for all 3 colors)
  * @return Current state of timer controlled send
  */
PWM_STATE_t pwm_SendValues(RGB_t colors)
{
	if(isInitiated)
	{
		if(!isRunning)
		{
			uint16_t index = 0;
			uint32_t color = ((((uint8_t)colors.green)<<16) | (((uint8_t)colors.red)<<8) | (((uint8_t)colors.blue)));

			for(int i=0;i<240;i++) //Pull down DO for about 280us
			{
				pwmData[index] = 0;
				index++;
			}
			for(int i=23; i>=0; i--) //Format color values
			{
				if(color&(1<<i))
				{
					pwmData[index] = 29; //0,9 us (29/40) "1"
				}
				else
				{
					pwmData[index] = 11; //0,35 us (11/40) "0"
				}
				index++;
			}
			for(int i=0;i<240;i++) //Pull down DO for about 280us
			{
				pwmData[index] = 0;
				index++;
			}
			if(HAL_TIM_PWM_Start_DMA(htim_local, TIM_CHANNEL_1, (uint32_t*)pwmData, sizeof(pwmData)/sizeof(pwmData[0]))!=HAL_OK) //start transmission to RGB LED
			{
				return PROBLEM;
			}
			else
			{
				isRunning = true;
				return RUNNING;
			}
		}
		else return STOPPED;

	}
	else
		return PROBLEM;
}
/**
  * @brief Transitions through all RGB colors in nice animation
  * @param None
  * @return PWM_STATE current state of timer while sending colors
  */
PWM_STATE_t pwm_StartUpAnimation(void)
{
	static RGB_t animation_Colors;
	uint16_t speed = 15;
	uint8_t brightness = 30;
	if(animation_Colors.red<brightness && animation_Colors.green == 0 && animation_Colors.blue == 0) //R
	{
		animation_Colors.red++;
		HAL_Delay(speed);
	}
	else if(animation_Colors.green<brightness && animation_Colors.red == brightness && animation_Colors.blue == 0) //RG
	{
		animation_Colors.green++;
		HAL_Delay(speed);
	}
	else if(animation_Colors.red>0 && animation_Colors.green == brightness && animation_Colors.blue == 0) //G
	{
		animation_Colors.red--;
		HAL_Delay(speed);
	}
	else if(animation_Colors.blue<brightness && animation_Colors.green == brightness && animation_Colors.red == 0) //GB
	{
		animation_Colors.blue++;
		HAL_Delay(speed);
	}
	else if(animation_Colors.green>0 && animation_Colors.blue == brightness && animation_Colors.red == 0) //B
	{
		animation_Colors.green--;
		HAL_Delay(speed);
	}
	else if(animation_Colors.red<brightness && animation_Colors.green == 0 && animation_Colors.blue == brightness) //RB
	{
		animation_Colors.red++;
		HAL_Delay(speed);
	}
	else if(animation_Colors.green<brightness && animation_Colors.red == brightness && animation_Colors.blue == brightness) //RGB
	{
		animation_Colors.green++;
		HAL_Delay(speed);
	}
	else if(animation_Colors.red>0) //0
	{
		animation_Colors.red--;
		animation_Colors.green--;
		animation_Colors.blue--;
		HAL_Delay(speed);
		if(animation_Colors.red == 1 && animation_Colors.green == 1 && animation_Colors.blue == 1)
		{
			animation_Colors.red = 0;
			animation_Colors.green = 0;
			animation_Colors.blue = 0;
			pwm_SendValues(animation_Colors);
			return STOPPED;
		}
	}
	while(pwm_SendValues(animation_Colors)==RUNNING)
	{
		;
	}
	return RUNNING;

}

/**
  * @brief Stops the timer controlled DMA when finished
  * @param TIM_HandleTypeDef
  * @return None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) //DMA finished callback
{
	HAL_TIM_PWM_Stop_DMA(htim_local, TIM_CHANNEL_1); //Stop DMA again
	isRunning = false;
}
