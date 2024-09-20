/**
  ******************************************************************************
  * @file    i2c_driver.c
  * @author  Mathias Bohle
  * @date 	 13.12.2023
  * @brief   Controls I2C communication with VELM3328 sensor on Color 10 click.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "i2c_driver.h"


/* Defines -------------------------------------------------------------------*/
//Values
const uint8_t I2C_SLAVE_ADDR = 0x10;
const uint8_t I2C_SLAVE_DEV_ID = 0x28;

//Commands for Registers
const uint8_t I2C_CMD_CFG_REG = 0x00;
const uint8_t I2C_CMD_CLEAR_REG = 0x04;
const uint8_t I2C_CMD_RED_REG = 0x05;
const uint8_t I2C_CMD_GREEN_REG = 0x06;
const uint8_t I2C_CMD_BLUE_REG = 0x07;
const uint8_t I2C_CMD_IR_REG = 0x08;
const uint8_t I2C_CMD_DEV_ID_REG = 0x0C;

//Configuration
const uint16_t I2C_CFG_HDR_THIRD = 0x0040;
const uint16_t I2C_CFG_HDR_ONE = 0x0000;
const uint16_t I2C_CFG_INTEGRATION_TIME_50MS = 0x0000;
const uint16_t I2C_CFG_INTEGRATION_TIME_100MS = 0x0010;
const uint16_t I2C_CFG_INTEGRATION_TIME_200MS = 0x0020;
const uint16_t I2C_CFG_INTEGRATION_TIME_400MS = 0x0030;
const uint16_t I2C_CFG_MODE_AUTO = 0x0000;
const uint16_t I2C_CFG_MODE_MANUAL = 0x0008;
const uint16_t I2C_CFG_TRIGGER_NONE = 0x0000;
const uint16_t I2C_CFG_TRIGGER_ONCE = 0x0004;
const uint16_t I2C_CFG_PWR_ON = 0x0000;
const uint16_t I2C_CFG_PWR_OFF = 0x8001;
const uint16_t I2C_CFG_MEAS_ALL_CHANNELS = 0x0000;
const uint16_t I2C_CFG_MEAS_G_C_IR_ONLY = 0x4000;
const uint16_t I2C_CFG_GAIN1_X1 = 0x0000;
const uint16_t I2C_CFG_GAIN1_X2 = 0x1000;
const uint16_t I2C_CFG_GAIN1_X4 = 0x2000;
const uint16_t I2C_CFG_GAIN1_HALF = 0x0C00;
const uint16_t I2C_CFG_GAIN2_X1 = 0x0000;
const uint16_t I2C_CFG_GAIN2_X2 = 0x0400;
const uint16_t I2C_CFG_GAIN2_X4 = 0x0800;

/* Globals -------------------------------------------------------------------*/
uint8_t address = 0x10;
uint8_t HIGH_LOW_buffer[2] = { 0 };
I2C_HandleTypeDef* hi2c_local = NULL;

/* Functions -----------------------------------------------------------------*/
/**
  * @brief Gets copy of I2C_HandleTypeDef from main
  * @param I2C_HandleTypeDef
  * @return None
  */
void i2c_Init(I2C_HandleTypeDef* hi2c)
{
	hi2c_local = hi2c;
}
/**
  * @brief Verifies the Device ID of the VEML3328
  * @param None
  * @return _Bool, true if DeviceID was correct.
  */
_Bool i2c_verifyDeviceID()
{
	HAL_I2C_Mem_Read(hi2c_local, (I2C_SLAVE_ADDR<<1),I2C_CMD_DEV_ID_REG , 1, HIGH_LOW_buffer, 1, HAL_MAX_DELAY);
	if(HIGH_LOW_buffer[0] == I2C_SLAVE_DEV_ID)
		return true;
	else return false;
}
/**
  * @brief Reads value from CLEAR Register of VEML3328
  * @param None
  * @return 16-Bit Value
  */
uint16_t i2c_getClear(void)
{
	HAL_I2C_Mem_Read(hi2c_local, (I2C_SLAVE_ADDR<<1), I2C_CMD_CLEAR_REG, 1, HIGH_LOW_buffer, 2, HAL_MAX_DELAY);
	return (uint16_t)(HIGH_LOW_buffer[0] | (HIGH_LOW_buffer[1]<<8));
}
/**
  * @brief Reads value from RED Register of VEML3328
  * @param None
  * @return 16-Bit Value
  */
uint16_t i2c_getRed(void)
{
	HAL_I2C_Mem_Read(hi2c_local, (I2C_SLAVE_ADDR<<1), I2C_CMD_RED_REG, 1, HIGH_LOW_buffer, 2, HAL_MAX_DELAY);
	return (uint16_t)(HIGH_LOW_buffer[0] | (HIGH_LOW_buffer[1]<<8));
}
/**
  * @brief Reads value from GREEN Register of VEML3328
  * @param None
  * @return 16-Bit Value
  */
uint16_t i2c_getGreen(void)
{
	HAL_I2C_Mem_Read(hi2c_local, (I2C_SLAVE_ADDR<<1), I2C_CMD_GREEN_REG, 1, HIGH_LOW_buffer, 2, HAL_MAX_DELAY);
	return (uint16_t)(HIGH_LOW_buffer[0] | (HIGH_LOW_buffer[1]<<8));
}
/**
  * @brief Reads value from BLUE Register of VEML3328
  * @param None
  * @return 16-Bit Value
  */
uint16_t i2c_getBlue(void)
{
	HAL_I2C_Mem_Read(hi2c_local, (I2C_SLAVE_ADDR<<1), I2C_CMD_BLUE_REG, 1, HIGH_LOW_buffer, 2, HAL_MAX_DELAY);
	return (uint16_t)(HIGH_LOW_buffer[0] | (HIGH_LOW_buffer[1]<<8));
}
/**
  * @brief Reads value from INFRARED Register of VEML3328
  * @param None
  * @return 16-Bit Value
  */
uint16_t i2c_getIR(void)
{
	HAL_I2C_Mem_Read(hi2c_local, (I2C_SLAVE_ADDR<<1), I2C_CMD_IR_REG, 1, HIGH_LOW_buffer, 2, HAL_MAX_DELAY);
	return (uint16_t)(HIGH_LOW_buffer[0] | (HIGH_LOW_buffer[1]<<8));
}
/**
  * @brief sends Configuration to VEML3328 Color Sensor
  * @param None
  * @return None
  */
_Bool i2c_startUp()
{
	//Identical to default Configuration, apart from Integration time
	uint16_t config = (I2C_CFG_PWR_ON | I2C_CFG_MEAS_ALL_CHANNELS | I2C_CFG_GAIN1_X1 | I2C_CFG_GAIN2_X1 | I2C_CFG_HDR_ONE | I2C_CFG_INTEGRATION_TIME_100MS | I2C_CFG_MODE_AUTO | I2C_CFG_TRIGGER_NONE);
	HIGH_LOW_buffer[0] = config & 0xFF;
	HIGH_LOW_buffer[1] = config >> 8;

	HAL_I2C_Mem_Write(hi2c_local, (I2C_SLAVE_ADDR<<1), I2C_CMD_CFG_REG, 1, HIGH_LOW_buffer, 2, HAL_MAX_DELAY);
	HAL_Delay(1);

	HAL_I2C_Mem_Read(hi2c_local, (I2C_SLAVE_ADDR<<1), I2C_CMD_CFG_REG, 1, HIGH_LOW_buffer, 2, HAL_MAX_DELAY);
	uint16_t check_config = (uint16_t)(HIGH_LOW_buffer[0] | (HIGH_LOW_buffer[1]<<8));

	return (config == check_config);
}

