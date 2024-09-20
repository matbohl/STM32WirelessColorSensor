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
#include "pwm_driver.h"
#include "i2c_driver.h"

/* Globals -------------------------------------------------------------------*/
osThreadId_t measurementTaskHandle;

osThreadAttr_t Task_attributes= {
		.stack_size = TASK_STACK_SIZE,
		.priority = TASK_PRIORITY,
};

osMessageQueueId_t colorUpdateQueueHandle;

const osMessageQueueAttr_t colorUpdateQueue_attributes = {
  .name = "ColorUpdateQueue"
};

osMessageQueueId_t MeasurementQueueHandle;

const osMessageQueueAttr_t MeasurementQueue_attributes = {
  .name = "MeasurementQueue"
};

osEventFlagsId_t colorUpdateEventHandle;

const osEventFlagsAttr_t colorUpdateEvent_attributes = {
  .name = "ColorUpdate"
};

/* Functions -----------------------------------------------------------------*/
/**
 *  @brief Initiates all tasks, message queues and events
 *  @param None
 *  @return TASK_CREATION_t to make sure task was created, check for TASK_ERROR
 */
TASK_CREATION_t init_Tasks(void)
{
	colorUpdateQueueHandle = osMessageQueueNew(2, sizeof(RGB_t), &colorUpdateQueue_attributes);
	if(colorUpdateQueueHandle == NULL)
		return TASKS_ERROR;

	colorUpdateEventHandle = osEventFlagsNew(&colorUpdateEvent_attributes);
	if(colorUpdateEventHandle == NULL)
		return TASKS_ERROR;

	MeasurementQueueHandle = osMessageQueueNew(2, sizeof(struct MEASUREMENT_S), &MeasurementQueue_attributes);
	if(MeasurementQueueHandle == NULL)
		return TASKS_ERROR;

	Task_attributes.name = "measurementTask";
	measurementTaskHandle = osThreadNew(StartMeasurementTask,NULL,&Task_attributes);
	if(osThreadGetState(measurementTaskHandle)==osThreadError)
		return TASKS_ERROR;

	return TASKS_CREATED;
}
/**
 *  @brief MeasurementTask waits for MEASUREMENT_NEEDED flag, measures and sets
 *  	   MEASUREMENT_DONE flag.
 *  @param None
 *  @return None
 */
void StartMeasurementTask(void *argument)
{
	uint32_t measure_flags = 0;
	struct MEASUREMENT_S values;

	for(;;)
	{
		measure_flags = osEventFlagsWait(colorUpdateEventHandle,MEASUREMENT_NEEDED,osFlagsNoClear,osWaitForever);
		if(measure_flags == MEASUREMENT_NEEDED)
		{
			osEventFlagsClear(colorUpdateEventHandle, MEASUREMENT_NEEDED);
			values.red = i2c_getRed();
			values.green = i2c_getGreen();
			values.blue = i2c_getBlue();
			values.infrared = i2c_getIR();
			values.clear = i2c_getClear();
			osMessageQueuePut(MeasurementQueueHandle, &values, 0, 0);
			osEventFlagsSet(colorUpdateEventHandle, MEASUREMENT_DONE);
		}
	}





}

