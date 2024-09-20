/**
  ******************************************************************************
  * @file    tasks.c
  * @author  Mathias Bohle
  * @date 	 02.05.2024
  * @brief   Creates all additional tasks.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "tasks.h"
#include "oled_lib.h"
#include "io_driver.h"
#include "adc_driver.h"
#include "math.h"

/* Globals -------------------------------------------------------------------*/
osThreadId_t ioTaskHandle;
osThreadId_t oledTaskHandle;

osThreadAttr_t Task_attributes= {
		.stack_size = TASK_STACK_SIZE,
		.priority = TASK_PRIORITY,
};

osMessageQueueId_t ScrollValueQueueHandle;

const osMessageQueueAttr_t ScrollValueQueue_attributes = {
  .name = "ScrollValueQueue"
};

osMessageQueueId_t MeasurementQueueHandle;

const osMessageQueueAttr_t MeasurementQueue_attributes = {
  .name = "MeasurementQueue"
};

osMessageQueueId_t ColorUpdateQueueHandle;

const osMessageQueueAttr_t ColorUpdateQueue_attributes = {
  .name = "ColorUpdateQueue"
};

osEventFlagsId_t ioUpdateEventHandle;

const osEventFlagsAttr_t ioUpdateEvent_attributes = {
  .name = "ioUpdate"
};

osEventFlagsId_t colorUpdateEventHandle;

const osEventFlagsAttr_t colorUpdateEvent_attributes = {
  .name = "ioUpdate"
};



/* Functions -----------------------------------------------------------------*/
/**
 *  @brief initiates all tasks, message queues and events
 *  @param None
 *  @return TASK_CREATION_t to make sure task was created, check for TASK_ERROR
 */
TASK_CREATION_t init_Tasks(void)
{
	ScrollValueQueueHandle = osMessageQueueNew(2, sizeof(ScrollValue_t), &ScrollValueQueue_attributes);
	if(ScrollValueQueueHandle == NULL)
		 return TASKS_ERROR;

	MeasurementQueueHandle = osMessageQueueNew(2, sizeof(struct MEASUREMENT_S), &MeasurementQueue_attributes);
	if(MeasurementQueueHandle == NULL)
		 return TASKS_ERROR;

	ColorUpdateQueueHandle = osMessageQueueNew(2, sizeof(RGB_t), &ColorUpdateQueue_attributes);
	if(ColorUpdateQueueHandle == NULL)
		return TASKS_ERROR;

	ioUpdateEventHandle = osEventFlagsNew(&ioUpdateEvent_attributes);
	if(ioUpdateEventHandle == NULL)
		return TASKS_ERROR;

	colorUpdateEventHandle = osEventFlagsNew(&colorUpdateEvent_attributes);
	if(colorUpdateEventHandle == NULL)
		return TASKS_ERROR;

	Task_attributes.name = "ioTask";
	ioTaskHandle = osThreadNew(StartIOTask,NULL,&Task_attributes);
	if(osThreadGetState(ioTaskHandle)==osThreadError)
		return TASKS_ERROR;

	Task_attributes.name = "oledTask";
	oledTaskHandle = osThreadNew(StartOLEDTask,NULL,&Task_attributes);
	if(osThreadGetState(oledTaskHandle)==osThreadError)
		return TASKS_ERROR;

	return TASKS_CREATED;
}
/**
 *  @brief io Task that periodically looks for button press and reads ADC value, if ADC value changed
 *  	   or button is pressed it correctly sets ioUpdate event and puts new adc value in message queue.
 *  @param None
 *  @return None
 */
void StartIOTask(void *argument)
{
	ScrollValue_t ADC_value;
	ADC_value.value = 0;
	ADC_value.scaledValue = 0;
	static uint16_t OldValue = 0;
	for(;;)
	{
		if(io_readButtonWithDebounce()==PRESSED)
		{
			osEventFlagsSet(ioUpdateEventHandle, CLICK);
			osDelay(10);
		}
		adc_getVal(&ADC_value.value, 12);
		ADC_value.scaledValue = adc_scale(30, 95, ADC_value.value);
		if(ADC_value.value>(OldValue+1) || ADC_value.value<(OldValue-1))
		{
			osMessageQueuePut(ScrollValueQueueHandle, &ADC_value, 0,0);
			osEventFlagsSet(ioUpdateEventHandle,SCROLL);
			OldValue = ADC_value.value;
		}
		osDelay(50);
	}
}
/**
 *  @brief OLED Task handles complete menu, receives input from ioTask and communicates
 *  	   with Controller Task.
 *  @param None
 *  @return None
 */
void StartOLEDTask(void *argument)
{
	struct MEASUREMENT_S CurrentValues;
	CurrentValues.red = 0;
	CurrentValues.green = 0;
	CurrentValues.blue = 0;
	CurrentValues.clear = 0;
	CurrentValues.infrared = 0;

	RGB_t CurrentColors;
	CurrentColors.red = 0;
	CurrentColors.green = 0;
	CurrentColors.blue = 0;

	ScrollValue_t ScrollValue;
	ScrollValue.value = 0;
	ScrollValue.scaledValue=30;

	MENU_STATE_t state = WAITING;
	MENU_ITEM_t item = NO_ITEM;
	SUBMENU_STATE_t sub_state = NONE;
	SET_COLOR_STATE_t sub_4_state = RED;

	char write_buffer [30];

	oled_loadingScreen();
	osDelay(1000);
	oled_continueMessage();

	uint32_t io_flags;
	for(;;)
		{
			if(state==WAITING)
				io_flags = osEventFlagsWait(ioUpdateEventHandle,BOTH,osFlagsNoClear,500);
			else
				io_flags = osEventFlagsWait(ioUpdateEventHandle,BOTH,osFlagsNoClear,osWaitForever);

			if(io_flags == osFlagsErrorTimeout)
			{
				static _Bool onoff = true;
				if(onoff)
				{
					oled_continueMessageDot(DOT_ON);
					onoff = false;
				}
				else
				{
					oled_continueMessageDot(DOT_OFF);
					onoff = true;
				}
				osEventFlagsClear(ioUpdateEventHandle, osFlagsErrorTimeout);
			}
			else if(io_flags == CLICK)
			{
				if(state==WAITING)
				{
					state=MAIN;
					oled_blankScreen();
					oled_drawMainMenu("Measurement","LUX + CCT","Get Color","Set Color" );
					osEventFlagsClear(ioUpdateEventHandle,CLICK);
				}
				else if(state == MAIN)
				{
					state = SUB;

					if(item == FIRST_ITEM)
					{
						osEventFlagsSet(colorUpdateEventHandle, MEASUREMENT_NEEDED);

						osMessageQueueGet(MeasurementQueueHandle, &CurrentValues, 0, osWaitForever);

						oled_drawItemMenu("MEASURE","AGAIN","BACK");

						snprintf( write_buffer, 30, "Red: %u", CurrentValues.red );
						oled_writeText( &write_buffer[0], 4, 14 );
						snprintf( write_buffer, 30, "Green: %u", CurrentValues.green );
						oled_writeText( &write_buffer[0], 4, 25 );
						snprintf( write_buffer, 30, "Blue: %u", CurrentValues.blue );
						oled_writeText( &write_buffer[0], 4, 36 );
						snprintf( write_buffer, 30, "Clear: %u", CurrentValues.clear );
						oled_writeText( &write_buffer[0], 4, 47 );
						snprintf( write_buffer, 30, "Infrared: %u", CurrentValues.infrared );
						oled_writeText( &write_buffer[0], 4, 58 );

					}
					else if(item == SECOND_ITEM)
					{
						osEventFlagsSet(colorUpdateEventHandle, MEASUREMENT_NEEDED);
						osMessageQueueGet(MeasurementQueueHandle, &CurrentValues, 0, osWaitForever);

						  oled_drawItemMenu("LUX + CCT","AGAIN","BACK");
		      	  		  //Calculation according to correct gain, integration time and sensitivity
		      	  		  snprintf( write_buffer, 30, "Intensity: %u.%ulux", (CurrentValues.green*192/1000),(CurrentValues.green*192/100)%10 );
		      	  		  oled_writeText( &write_buffer[0], 4, 25 );

		      	  		  //Calculation according to Application Guide of VEML3328
		      	  		  double CCT = 11179;
		      	  		  double CCTi=0;
		      	  		  if(CurrentValues.blue<1)
		      	  			  CCTi = CurrentValues.red+CurrentValues.green+1;
		      	  		  else CCTi = (CurrentValues.red +CurrentValues.green)/CurrentValues.blue;
		      	  		  CCT = CCT*pow(CCTi,-0.805); //math.h also uses a lot of memory
		      	  		  snprintf( write_buffer, 30, "Color Temp.: %uK", (uint16_t)CCT);
		      	  		  oled_writeText( &write_buffer[0], 4, 47 );
					}
					else if(item == THIRD_ITEM)
					{

						  oled_drawItemMenu("GET Color","AGAIN","BACK");

		      			  //Turn on RGB LED
		      			  CurrentColors.red = 230; //Attempting to have "white LED"
		      			  CurrentColors.green = 160;
		      			  CurrentColors.blue = 90;
		      			  osMessageQueuePut(ColorUpdateQueueHandle, &CurrentColors, 0, 0);
		      			  osEventFlagsSet(colorUpdateEventHandle,NEW_COLOR);

		      			  osDelay(200); //Integration time of sensor -> 100ms, so 200ms is enough time

		      			  osEventFlagsSet(colorUpdateEventHandle, MEASUREMENT_NEEDED);
		      			  osMessageQueueGet(MeasurementQueueHandle, &CurrentValues, 0, osWaitForever);

		      			  //Turn off RGB LED
		      	  		  CurrentColors.red = 0;
		      	  		  CurrentColors.green=0;
		      	  		  CurrentColors.blue=0;

		      	  		  osMessageQueuePut(ColorUpdateQueueHandle, &CurrentColors, 0, 0);
		      	  		  osEventFlagsSet(colorUpdateEventHandle,NEW_COLOR);

		      	  		  if(CurrentValues.clear>0)
		      	  		  {
		      	  		  //Scale Values to 8BIT (with max being "clear" value)
		      	  		  uint32_t scaledRed = CurrentValues.red*255;
		      	  			  scaledRed = scaledRed / CurrentValues.clear;
		      	  		  uint32_t scaledGreen = CurrentValues.green*255;

		      	  		  scaledGreen = scaledGreen / CurrentValues.clear;
		      	  		  uint32_t scaledBlue = CurrentValues.blue*255;
		      	  		  scaledBlue = scaledBlue / CurrentValues.clear;

		      	  		  CurrentColors.red = scaledRed;
		      	  		  CurrentColors.green = scaledGreen;
		      	  		  CurrentColors.blue = scaledBlue;
		      	  		  }
		      	  		  else
		      	  		  {
			      	  		  CurrentColors.red = 0;
			      	  		  CurrentColors.green = 0;
			      	  		  CurrentColors.blue = 0;
		      	  		  }

		      	  		  //Print values
		      	  		  snprintf( write_buffer, 30, "R:0x%.2X h",CurrentColors.red );
		      	  		  oled_writeText( &write_buffer[0], 52, 24 );

		      	  		  snprintf( write_buffer, 30, "G:0x%.2X h",CurrentColors.green );
		      	  		  oled_writeText( &write_buffer[0], 52, 37);

		      	  		  snprintf( write_buffer, 30, "B:0x%.2X h",CurrentColors.blue );
		      	  		  oled_writeText( &write_buffer[0], 52, 50 );

		      	  		  //Fill section of screen with measured color
		      			  oled_FillArea(4, 22, 44, 62, (((CurrentColors.red>>3) << 11) | ((CurrentColors.green>>2) << 5) | CurrentColors.blue >> 3));

		      			  CurrentColors.red = 0;
		      			  CurrentColors.green = 0;
		      			  CurrentColors.blue = 0;
					}
					else if(item == FOURTH_ITEM)
					{
		      			  oled_drawItemMenu("SET COLORS","  SET"," RED");

		      			  //Print Graphics and Values
		      			  oled_FillArea(12, 18, 29, 27, 0xFFFF);
		      			  snprintf( write_buffer, 30, "R:%3.u",CurrentColors.red );
		      			  oled_writeText( &write_buffer[0], 4, 17 );
		      			  oled_SetColorCursor(RED, CurrentColors.red);

		      			  oled_FillArea(12, 37, 29, 46, 0xFFFF);
		      			  snprintf( write_buffer, 30, "G:%3.u",CurrentColors.green );
		      			  oled_writeText( &write_buffer[0], 4, 35);
		      			  oled_SetColorCursor(GREEN, CurrentColors.green);

		      			  oled_FillArea(12, 57, 29, 66, 0xFFFF);
		      			  snprintf( write_buffer, 30, "B:%3.u",CurrentColors.blue );
		      			  oled_writeText( &write_buffer[0], 4, 55);
		      			  oled_SetColorCursor(BLUE, CurrentColors.blue);

		      			  sub_4_state = RED;
		      			  sub_state = SET_COLOR;

					}
					osEventFlagsClear(ioUpdateEventHandle,CLICK);
				}
				else if(state == SUB)
				{
					if(sub_state == BACK)
					{
						state = MAIN;
						oled_blankScreen();
						oled_drawMainMenu("Measurement","LUX + CCT","Get Color","Set Color" );
						osEventFlagsClear(ioUpdateEventHandle,CLICK);
					}
					else if(sub_state == SET_COLOR)
					{
						if(sub_4_state == RED)
						{
							oled_FillArea(50, 75, 93, 93, 0xFFFF);
							snprintf( write_buffer, 30, "GREEN");
							oled_writeText( &write_buffer[0], 55, 79 );
							osMessageQueuePut(ColorUpdateQueueHandle, &CurrentColors, 0, 0);
							osEventFlagsSet(colorUpdateEventHandle,NEW_COLOR);

							sub_4_state = GREEN;
						}
						else if(sub_4_state == GREEN)
						{
							oled_FillArea(50, 75, 93, 93, 0xFFFF);
							snprintf( write_buffer, 30, "BLUE");
							oled_writeText( &write_buffer[0], 60, 79 );
							osMessageQueuePut(ColorUpdateQueueHandle, &CurrentColors, 0, 0);
							osEventFlagsSet(colorUpdateEventHandle,NEW_COLOR);

							sub_4_state = BLUE;
						}
						else if(sub_4_state == BLUE)
						{
							oled_FillArea(50, 75, 93, 93, 0xFFFF);
							snprintf( write_buffer, 30, "BACK");
							oled_writeText( &write_buffer[0], 60, 79 );

							oled_FillArea(4, 75, 44, 93, 0xFFFF);
							snprintf( write_buffer, 30, "AGAIN" );
							oled_writeText( &write_buffer[0], 7, 79 );
							osMessageQueuePut(ColorUpdateQueueHandle, &CurrentColors, 0, 0);
							osEventFlagsSet(colorUpdateEventHandle,NEW_COLOR);

							sub_4_state = AGAIN_BACK;
						}
					osEventFlagsClear(ioUpdateEventHandle,CLICK);
					}
					else if(sub_state == REPEAT)
					{
						CurrentColors.red = 0;
						CurrentColors.green = 0;
						CurrentColors.blue = 0;

						CurrentValues.red = 0;
						CurrentValues.green = 0;
						CurrentValues.blue = 0;
						CurrentValues.infrared = 0;
						CurrentValues.clear = 0;

						state = MAIN;
						sub_4_state = RED;
						oled_blankScreen();
						osEventFlagsClear(ioUpdateEventHandle,CLICK);
						osEventFlagsSet(ioUpdateEventHandle, CLICK);
					}
				}

			}
			if(io_flags == SCROLL)
			{
				while(osMessageQueueGetCount(ScrollValueQueueHandle)>0)
				{
					osMessageQueueGet(ScrollValueQueueHandle, &ScrollValue, 0, 0);
				}
				if(state == MAIN)
				{
					if(ScrollValue.scaledValue<45)
					{
						item = FIRST_ITEM;
					}
					else if (ScrollValue.scaledValue>=45 && ScrollValue.scaledValue<65)
					{
						item = SECOND_ITEM;
					}
					else if(ScrollValue.scaledValue>=65 && ScrollValue.scaledValue<85)
					{
						item = THIRD_ITEM;
					}
					else if(ScrollValue.scaledValue>=85 && ScrollValue.scaledValue<95)
					{
						item = FOURTH_ITEM;
					}
					oled_highlightMainItem(item);
					oled_FillArea(91, 14, 94, 96, 0xFFFF);
					oled_FillArea(91, ScrollValue.scaledValue-15, 94, ScrollValue.scaledValue, 0x630C);
				}
				else if(state == SUB && sub_state !=SET_COLOR)
				{
					if(ScrollValue.scaledValue>63)
					 {
						sub_state = REPEAT;
					 }
					 else
					 {
						 sub_state = BACK;
					 }
					oled_highlightItemLR(sub_state);
				}
				else if(state == SUB && sub_state==SET_COLOR)
				{
					if(sub_4_state == RED)
					{
	  					  oled_SetColorCursor(RED, 255-ScrollValue.value);
	  					  oled_FillArea(12, 18, 29, 27, 0xFFFF);
	  					  snprintf( write_buffer, 30, "R:%3.u",255-ScrollValue.value);
	  					  oled_writeText( &write_buffer[0], 4, 17 );
	  					  CurrentColors.red = (255-ScrollValue.value);
					}
					else if(sub_4_state == GREEN)
					{
	  					  oled_SetColorCursor(GREEN, 255-ScrollValue.value);
	  					  oled_FillArea(12, 37, 29, 46, 0xFFFF);
	  					  snprintf( write_buffer, 30, "G:%3.u",255-ScrollValue.value);
	  					  oled_writeText( &write_buffer[0], 4, 35);
	  					  CurrentColors.green = (255-ScrollValue.value);
					}
					else if(sub_4_state == BLUE)
					{
	  					  oled_SetColorCursor(BLUE, 255-ScrollValue.value);
	  					  oled_FillArea(12, 57, 29, 66, 0xFFFF);
	  					  snprintf( write_buffer, 30, "B:%3.u",255-ScrollValue.value);
	  					  oled_writeText( &write_buffer[0], 4, 55);
	  					  CurrentColors.blue = (255-ScrollValue.value);
					}
					else if(sub_4_state == AGAIN_BACK)
					{
						if(ScrollValue.scaledValue>63)
						{
							sub_state = REPEAT;
						}
						else
						{
							sub_state = BACK;
						}
						oled_highlightItemLR(sub_state);
					}
				}

				osEventFlagsClear(ioUpdateEventHandle,SCROLL);
			}

		}
}


