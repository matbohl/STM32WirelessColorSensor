# Project - OLED Display Board
This Programm uses one Clickboard and a Serial to Telemetry Module. 
The OLED C Click has a 96x96 Pixel OLED Display, which is controlled by a SSD1351. The OLED-Display communicates via 4 wire SPI.
The Telemetry Module wirelessly transmits UART commands to the other board.
The implemented Features should be self explanatory, there even is a small manual shown in the start up sequence.  
To maneuver in the menu the potentiometer is used for scrolling and the button for selection.

## Configuartion
STM32L432KCUx microcontroller / Nucleo32 Board with ClickShield
- Telemetry Module (5V, GND, TX, RX on the RIGHT)
- OLED C Click is on the first mikrobus connector (1 / LEFT)

Specific Pin Configuration:
>**Serial Peripheral Interface (SPI1)**
>SPI1_MOSI <-> PB5
>SPI1_MISO <-> PB4
>SPI1_SCK <-> PB3
>OLED_CS <-> PB0
>OLED_CMD_DATA <-> PB1
>Prescaler -> 4
>Clock Polarity -> High
>Clock Phase -> 2 Edge

>**Button**
>BTN <-> PA3

>**Analog-Digital-Converter(ADC1)**
>ADC1_IN12 <-> PA7

>**Clickboard LEDs (Blue and Green)**
>RGB_B <-> PA4 
>GPIO output level -> High
>RGB_G <-> PA8
>GPIO output level -> High

>**Universal Synchronous Asynchronous Receiver Transmitter (USART1)**
>USART1_RX <-> PA9
>USART1_TX <-> PA10
Mode Asynchronous
Baud Rate 57600 Bits/s
Word Length 8Bits
Parity None
Stop Bits 1
NVIC: USART1 global interrupt ENABLED
Register Callback UART ENABLE

## Files
The functions have been split into multiple Files to keep the code somewhat modular. 

> **IO:** 
> io_driver.h
> io_driver.c

Functions for interacting with the button on the Clickshield.

> **ADC:** 
> adc_driver.h
> adc_driver.c

Reads value (in 8Bit) of potentiometer on Clickshield,
 
 > **OLED:** 
> oled_driver.h
> oled_driver.c
> 
 Interactions with OLED display. handles SPI and functions like, fill, draw, write and bitmaps,

> **OLED LIB:** 
> oled_lib.h
> oled_lib.c

 Abstraction library for the OLED display.

> **printf:** 
> printf.h
> printf.c

 Brings back traditional printf() functionality.

> **uart:** 
> uart.h
> uart.c

Handles UART Hardware (Receive to Idle)

> **tasks:** 
> tasks.h
> tasks.c

Generates all additional tasks (ControllerTask is in main.c)

## Tasks
The project was realized in FreeRTOS, different parts of the project where distributed into several Tasks. Intertask communication is handled via message queues and event/flags.

> **Controller Task:** 

Handles communication between other Tasks. 

> **IO Task:** 

Checks Button and Potentiometer periodically and updates OLED Task with current Inputs.

> **OLED Task:** 

Receives Information from IO Task and handles Menu accordingly. Also communicates with Controller Task in order to Communicate with other Board.

## Problems
The button is directly connected with the enable Pin of the OLED display, so now the display goes blank for the duration that the button is pushed.  