/**
  ******************************************************************************
  * @file    io_driver.c
  * @author  Mathias Bohle
  * @date 	 01.12.2023
  * @brief   Provides functions to interact with IOs like the button of the
  * ClickShield
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "io_driver.h"
#include "cmsis_os.h"


/* Functions -----------------------------------------------------------------*/
/**
  * @brief This function reads button on Click Shield. Bouncing behavior is compensated for.
  * @param None
  * @return NOT_PRESSED while button not pressed; PRESSED when button was pressed and is released again.
  */
BUTTON_STATE_t io_readButtonWithDebounce(void)
{
	static _Bool buttonPressedBefore;
		//Button was pressed for first time
		if(!HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)&&!buttonPressedBefore)
		{
			//Wait for end of possible bouncing behavior
			osDelay(1);
			//Stopped bouncing and still pressed?
			if(!HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin))
			{
				//Button was pressed and is still pressed
				buttonPressedBefore = true;
			}
		}
		//Button finally released again?
		else if(HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)&&buttonPressedBefore)
		{
			//return true and reset button state variable
			buttonPressedBefore=0;
			return PRESSED;
		}
		return NOT_PRESSED;
}

