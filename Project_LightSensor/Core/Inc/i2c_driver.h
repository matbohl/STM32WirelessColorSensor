/**
  ******************************************************************************
  * @file    i2c_driver.h
  * @author  Mathias Bohle
  * @date 	 13.12.2023
  * @brief   Controls I2C communication with VELM3328 sensor on Color 10 click.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/
//Values
extern const uint8_t I2C_SLAVE_ADDR;
extern const uint8_t I2C_SLAVE_DEV_ID;

//Commands for Registers
extern const uint8_t I2C_CMD_CFG_REG;
extern const uint8_t I2C_CMD_CLEAR_REG;
extern const uint8_t I2C_CMD_RED_REG;
extern const uint8_t I2C_CMD_GREEN_REG;
extern const uint8_t I2C_CMD_BLUE_REG;
extern const uint8_t I2C_CMD_IR_REG;
extern const uint8_t I2C_CMD_DEVID_REG;

//Configuration
extern const uint16_t I2C_CFG_HDR_THIRD;
extern const uint16_t I2C_CFG_HDR_ONE;
extern const uint16_t I2C_CFG_INTEGRATION_TIME_50MS;
extern const uint16_t I2C_CFG_INTEGRATION_TIME_100MS;
extern const uint16_t I2C_CFG_INTEGRATION_TIME_200MS;
extern const uint16_t I2C_CFG_INTEGRATION_TIME_400MS;
extern const uint16_t I2C_CFG_MODE_AUTO;
extern const uint16_t I2C_CFG_MODE_MANUAL;
extern const uint16_t I2C_CFG_TRIGGER_ONCE;
extern const uint16_t I2C_CFG_TRIGGER_NONE;
extern const uint16_t I2C_CFG_PWR_ON;
extern const uint16_t I2C_CFG_PWR_OFF;
extern const uint16_t I2C_CFG_MEAS_ALL_CHANNELS;
extern const uint16_t I2C_CFG_MEAS_G_C_IR_ONLY;
extern const uint16_t I2C_CFG_GAIN1_X1;
extern const uint16_t I2C_CFG_GAIN1_X2;
extern const uint16_t I2C_CFG_GAIN1_X4;
extern const uint16_t I2C_CFG_GAIN1_HALF;
extern const uint16_t I2C_CFG_GAIN2_X1;
extern const uint16_t I2C_CFG_GAIN2_X2;
extern const uint16_t I2C_CFG_GAIN2_X4;


/* Function Prototypes -------------------------------------------------------*/
void i2c_Init(I2C_HandleTypeDef* hi2c);
_Bool i2c_verifyDeviceID(void);
_Bool i2c_startUp(void);
uint16_t i2c_getClear(void);
uint16_t i2c_getRed(void);
uint16_t i2c_getGreen(void);
uint16_t i2c_getBlue(void);
uint16_t i2c_getIR(void);

#endif /* INC_I2C_DRIVER_H_ */
