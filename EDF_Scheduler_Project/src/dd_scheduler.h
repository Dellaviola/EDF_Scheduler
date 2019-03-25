/*
 * dd_scheduler.h
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */

#ifndef DD_SCHEDULER_H_
#define DD_SCHEDULER_H_

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"
/* Program Includes */
#include "list.h"
#include "options.h"

/*-----------------------------------------------------------*/

typedef struct Task_param_s{

	TickType_t deadline;
	TickType_t deadlinetick;
	TaskFunction_t task;
	char name[64];

}Task_param_s;

typedef enum {
	CREATE,
	DELETE,
	REQUEST_ACTIVE,
	REQUEST_OVERDUE,
	UPDATE_ACTIVE
} MessageType_t;

typedef struct Scheduler_request_message{

	MessageType_t MessageType;

}Scheduler_request_message;

typedef struct Task_create_message{

	MessageType_t MessageType;
	TickType_t Deadline;
	TaskHandle_t TaskHandle;
	TaskHandle_t OwnerHandle;

}Task_create_message;

typedef struct Task_delete_message{

	MessageType_t MessageType;
	TaskHandle_t TaskHandle;
	TaskHandle_t OwnerHandle;

}Task_delete_message;


typedef struct ID_message{

	MessageType_t MessageType;

}ID_message;

typedef union DD_Message{

	Scheduler_request_message RequestMessage;
	Task_create_message CreateMessage;
	Task_delete_message DeleteMessage;
	ID_message ID;

}DD_message;

/*-----------------------------------------------------------*/

// Global scheduler queue
xQueueHandle SchedulerQueue;
xQueueHandle ListQueue;

// Timer Handle

TimerHandle_t xTimers[5];

// Mutex
SemaphoreHandle_t xFunctionMutex;

/*-----------------------------------------------------------*/

TaskHandle_t dd_tcreate(Task_param_s);
void dd_delete(TaskHandle_t);
uint32_t dd_return_active_list(const TaskList*);
uint32_t dd_return_overdue_list(const TaskList*);

/*-----------------------------------------------------------*/

#endif /* DD_SCHEDULER_H_ */
