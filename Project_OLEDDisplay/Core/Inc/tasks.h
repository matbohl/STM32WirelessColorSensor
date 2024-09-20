/**
  ******************************************************************************
  * @file    tasks.h
  * @author  Mathias Bohle
  * @date 	 02.05.2024
  * @brief   Creates io task and oled task
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
	CLICK = 1,
	SCROLL = 2,
	BOTH = 3
}IO_FLAG_t;

typedef enum {
	MEASUREMENT_NEEDED = 1,
	MEASUREMENT_DONE = 2,
	NEW_COLOR = 4,
	ALL_THREE = 7
}MEASUREMENT_FLAG_t;

typedef struct ScrollValue
{
	uint16_t value;
	uint16_t scaledValue;
}ScrollValue_t;

struct MEASUREMENT_S{
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint16_t clear;
	uint16_t infrared;
};

/* Globals -------------------------------------------------------------------*/
extern osThreadId_t ioTaskHandle;
extern osThreadId_t oledTaskHandle;

extern osThreadAttr_t Task_attributes;

extern osMessageQueueId_t ScrollValueQueueHandle;

extern const osMessageQueueAttr_t ScrollValueQueue_attributes;

extern osMessageQueueId_t ColorUpdateQueueHandle;

extern const osMessageQueueAttr_t ColorUpdateQueue_attributes;

extern osMessageQueueId_t MeasurementQueueHandle;

extern const osMessageQueueAttr_t MeasurementQueue_attributes;

extern osEventFlagsId_t ioUpdateEventHandle;

extern const osEventFlagsAttr_t ioUpdateEvent_attributes;

extern osEventFlagsId_t colorUpdateEventHandle;

extern const osEventFlagsAttr_t colorUpdateEvent_attributes;

/* Defines -------------------------------------------------------------------*/
#define TASK_PRIORITY (osPriority_t) osPriorityBelowNormal

#define TASK_STACK_SIZE 128 * 4 //512 Byte

/* Function Prototypes -------------------------------------------------------*/

TASK_CREATION_t init_Tasks(void);
void StartIOTask(void *argument);
void StartOLEDTask(void *argument);

#endif /* INC_TASKS_H_ */
