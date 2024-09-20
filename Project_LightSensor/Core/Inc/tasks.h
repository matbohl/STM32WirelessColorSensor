/**
  ******************************************************************************
  * @file    tasks.h
  * @author  Mathias Bohle
  * @date 	 02.05.2024
  * @brief   Creates all additional tasks.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_TASKS_H_
#define INC_TASKS_H_

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"

/*Type Definitions -----------------------------------------------------------*/
typedef enum {
	TASKS_CREATED = 0,
	TASKS_ERROR = 1,
}TASK_CREATION_t;

typedef enum {
	MEASUREMENT_NEEDED = 1,
	MEASUREMENT_DONE = 2,
	NEW_COLOR = 4,
	DONE_OR_NEW_COLOR = 6
}MEASUREMENT_FLAG_t;

struct MEASUREMENT_S{
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint16_t clear;
	uint16_t infrared;
};

/* Globals -------------------------------------------------------------------*/
extern osThreadId_t measurementTaskHandle;

extern osThreadAttr_t Task_attributes;

extern osMessageQueueId_t colorUpdateQueueHandle;

extern osMessageQueueId_t MeasurementQueueHandle;

extern const osMessageQueueAttr_t colorUpdateQueue_attributes;

extern const osMessageQueueAttr_t MeasurementQueue_attributes;

extern osEventFlagsId_t colorUpdateEventHandle;

extern const osEventFlagsAttr_t colorUpdateEvent_attributes;

/* Defines -------------------------------------------------------------------*/
#define TASK_PRIORITY (osPriority_t) osPriorityBelowNormal

#define TASK_STACK_SIZE 128 * 4 //512 Byte

/* Function Prototypes -------------------------------------------------------*/

TASK_CREATION_t init_Tasks(void);
void StartMeasurementTask(void *argument);



#endif /* INC_TASKS_H_ */
