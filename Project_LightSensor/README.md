# Project - Light Sensor Board
This Programm uses one Clickboard and a Serial to Telemetry Module. The Color 10 Click contains a VEML3328 Color Sensor and a single W2812 RGB LED. 
The Telemetry Module wirelessly transmits UART commands to the other board.
the Color Sensor is connected wia I2C and the RGB LED uses only one Pin and gets the color values via single NZR communication based protocol. 
The implemented Features should be self explanatory, there even is a small manual shown in the start up sequence.  
To maneuver in the menu the potentiometer is used for scrolling and the button for selection.

## Configuartion
STM32L432KCUx microcontroller / Nucleo32 Board with ClickShield
- Color 10 Click on the second mikrobus connector (2 / RIGHT)
- Telemetry Module (5V, GND, TX, RX on the RIGHT)

Specific Pin Configuration:
>**Inter-Integrated Circuit (I2C1)**
>I2C1_SDA <-> PB7
>I2C1_SCL <-> PB6

>**Timer (TIM2 for WS2812)**
>Channel 1 -> PWM Generation CH1 <-> PA5
>Prescaler -> 0
>Counter Period -> 39
>Mode -> PWM mode 1
>CH Polarity -> High

>**Direct Memory Access (DMA1 for TIM2 PWM)**
>DMA Request -> TIM2_CH1
>Channel -> DMA1 Channel 5
>Direction -> Memory to Peripheral
>Priority -> Low
>Mode -> Normal
>Increment Address  [O] Peripheral [X] Memory
>Data Width -> Word - Word

>**Clickboard LEDs (Blue and Green)**
>RGB_B <-> PA4 
>GPIO output level -> High
>RGB_G <-> PA8
>GPIO output level -> High

>**SB18 -> PA6 connected with PB6**
>PA6 -> GPIO_Input (so it does not interfere with SCL

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

> **PWM:** 
 pwm_driver.h
 pwm_driver.c
 
Creates PWM with differing duty cycles for communication with LED (controlled via DMA).

> **I2C:** 
> i2c_driver.h
> i2c_driver.c

Handles I2C based communication with color sensor,
 
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


> **Measurement Task:** 

Waits for MEASUREMENT_NEEDED flag and communicates with sensor, then sets MEASUREMENT_DONE flag. 

## Problems
While programming i stumbled upon some weird issues which are hardware based and cannot be fixed without soldering,
The DI (Data In) Pin of the WS2812 LED is connected to the I2C SCL Pin of the VEML3328 Sensor by a Solder Bridge (SB16: PB6 connected to PA6 ). 
The DI Pin is pulled up by Resistors on the Click Board also. This took me a long time to figure out.  Luckily the LED mostly ignores I2C communication and I am not changeing the LED color while communicating with the color sensor. 
It took a long time to get the single LED working though.