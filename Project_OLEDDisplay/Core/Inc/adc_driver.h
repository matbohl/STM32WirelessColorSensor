/**
  ******************************************************************************
  * @file    adc_driver.h
  * @author  Mathias Bohle
  * @date 	 01.12.2023
  * @brief   Provides functions to configure and convert analog values from
  * 		 portPin
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_ADC_DRIVER_H_
#define INC_ADC_DRIVER_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/* Function Prototypes -------------------------------------------------------*/
void adc_Init(ADC_HandleTypeDef* hadc);
uint8_t adc_configure(uint8_t portPin);
uint8_t adc_getVal(uint16_t *pValue, uint8_t portPin);
uint16_t adc_scale(uint16_t min, uint16_t max, uint16_t input);

#endif /* INC_ADC_DRIVER_H_ */
