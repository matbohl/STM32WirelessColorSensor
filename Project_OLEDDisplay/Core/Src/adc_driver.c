/**
  ******************************************************************************
  * @file    adc_driver.c
  * @author  Mathias Bohle
  * @date 	 01.12.2023
  * @brief   Provides functions to configure and convert analog values from
  * 		 portPin
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "adc_driver.h"

/* Globals -------------------------------------------------------------------*/
static _Bool adc_isConfigured = false;
static ADC_HandleTypeDef* hadc_local = NULL;
static uint8_t portPin_copy = 0;

/* Functions -----------------------------------------------------------------*/
/**
 * 	@brief Initiates the adc.
 *  @param ADC_HandleTypeDef* adcToRead
 *  @return None.
 */
void adc_Init(ADC_HandleTypeDef* hadc)
{
	hadc_local = hadc;
	adc_isConfigured = true;
}
/**
 * 	@brief Function _adc_configure() initializes the ADC for single conversion
 *         of the given portPin
 *  @param portPin is the port pin that shall be converted (Only portPins that are not used yet).
 *  @returns 0 upon success,
 *  		 1 for Error with ADC_Init,
 *  		 2 for Error with portPin,
 *  		 3 for Errors with ADC_ConfigChannel
 *   */
uint8_t adc_configure(uint8_t portPin)
{
	if (HAL_ADC_Init(hadc_local) != HAL_OK)
		return 1;
	//Regular channel config
	static ADC_ChannelConfTypeDef sConfig = {0};

	if(portPin == 6)
		sConfig.Channel = ADC_CHANNEL_6;
	else if(portPin == 12)
		sConfig.Channel = ADC_CHANNEL_12;
	else if(portPin == 15)
		sConfig.Channel = ADC_CHANNEL_15;
	else if(portPin == 16)
		sConfig.Channel = ADC_CHANNEL_16;
	else if(portPin == 17)
		sConfig.Channel = ADC_CHANNEL_17;
	else if(portPin == 18)
		sConfig.Channel = ADC_CHANNEL_18;
	else
		return 2;

	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;

	//Try configuration
	if (HAL_ADC_ConfigChannel(hadc_local, &sConfig) == HAL_OK)
	{
		portPin_copy = portPin;
		return 0;
	}
	else return 3;
}

/**
 *  @brief Function _adc_getval() will start an analog to digital
 *         conversion of a provided portPin and write the result to
 *         the provided address.
 *  @param *pValue is the address where the result will be written to
 *  @param portPin is the port pin that shall be converted
 *  @returns 0 upon success,
 *  		 1 for Error with ADC_Start,
 *  		 2 for Errors with PollForConversion
 *  		 3 for Errors with wrong portPin
 */
uint8_t adc_getVal(uint16_t *pValue, uint8_t portPin)
{
	if(portPin == portPin_copy)
	{
		if(HAL_ADC_Start(hadc_local)!=HAL_OK)
				return 1;
			if(HAL_ADC_PollForConversion(hadc_local,1) == HAL_OK)
				{
					*pValue=HAL_ADC_GetValue(hadc_local);
					return 0;
				}
			else return 2;
	}
	else return 3;
}
/**
 *  @brief scales input value to a value between min and max
 *  @param min is minimal scale value
 *  @param max is maximum scale value
 *  @param input is value to be scaled
 *  @returns scaled value
 */
uint16_t adc_scale(uint16_t min, uint16_t max, uint16_t input)
{
	uint16_t range=max-min;
	uint16_t high=(range / 255)*input;
	uint16_t low =(range % 255)*input;
	return (min + high + (high+low+127)/255);
}
