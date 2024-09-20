/**
  ******************************************************************************
  * @file    io_driver.h
  * @author  Mathias Bohle
  * @date 	 05.12.2023
  * @brief   Provides functions to interact with IOs like the button of the
  * ClickShield
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_IO_DRIVER_H_
#define INC_IO_DRIVER_H_

/*Type Definitions -----------------------------------------------------------*/
typedef enum {
	PRESSED = 1,
	NOT_PRESSED = 0
} BUTTON_STATE_t;

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/*Function Prototypes --------------------------------------------------------*/
BUTTON_STATE_t io_readButtonWithDebounce(void);

#endif /* INC_IO_DRIVER_H_ */
