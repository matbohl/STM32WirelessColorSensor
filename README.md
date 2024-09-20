STM32 Wireless Color Sensor
This project is an RTOS based application using two STM32 Nucleo Boards on MikroE Clickshields. 
One MCU is connected to a OLED C Click Board and a wireless Telemetry Module.
The other MCU is connected to a Color 10 Click Board and a wireless Telemetry Module.

The two boards communicate wirelessly to transmit colorsensor data from the color sensor to the display.
Input is handled via a menu, implemented modes are: 
 - Raw Measurements
 - LUX and Correlated Color Temperature
 - set color
 - set color

The drivers for the click boards where inspired from the [mikroSDK click board librarys](https://github.com/MikroElektronika) and adapted for STM32 HAL / different use cases.
