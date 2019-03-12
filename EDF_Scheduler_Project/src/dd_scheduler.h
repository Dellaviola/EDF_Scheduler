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


#endif /* DD_SCHEDULER_H_ */

typedef struct{

	TickType_t deadline;
	TickType_t execution;

}Task_param_s;

typedef struct{

	TaskHandle_t handle;
	TickType_t deadline;
	TickType_t start;
	uint32_t task_type;
	struct Task_list_s* next;
	struct Task_list_s* prev;

}Task_list_s, *Task_list_sPtr;

typedef Task_list_s OverdueList, TaskList;
typedef enum {
	CREATE,
	DELETE,
	REQUEST_ACTIVE,
	REQUEST_OVERDUE
} MessageType_t;

typedef struct{

	MessageType_t MessageType;

}Scheduler_request_message;

typedef struct{

	MessageType_t MessageType;
	uint32_t index;
	TickType_t deadline;

}Task_create_message;

typedef struct{

	MessageType_t MessageType;
	TaskHandle_t handle;

}Task_create_response;

typedef struct{

	MessageType_t MessageType;
	TaskHandle_t handle;

}Task_delete_message;

typedef struct{

	MessageType_t MessageType;
	uint32_t retval;

}Task_delete_response;

typedef struct{

	MessageType_t MessageType;
	Task_list_s list;

}Task_request_response;

typedef union{

	Scheduler_request_message RequestMessage;
	Task_create_message CreateMessage;
	Task_delete_message DeleteMessage;
	Task_create_response CreateResponse;
	Task_delete_response DeleteResponse;
	Task_request_response TaskListResponse;

}DD_message;

/*-----------------------------------------------------------*/
TaskHandle_t dd_tcreate(Task_param_s);
uint32_t dd_delete(TaskHandle_t);
uint32_t dd_return_active_list(Task_list_s*);
uint32_t dd_return_overdue_list(Task_list_s*);

/*-----------------------------------------------------------*/
void _messageHandler(Scheduler_request_message*);
void _deadlineHandler();
