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

#include "list.h"
/*-----------------------------------------------------------*/
typedef struct Task_param_s{

	TickType_t deadline;
	TickType_t deadlinetick;
	TaskFunction_t task;
	char name[10];

}Task_param_s;

typedef enum {
	CREATE,
	DELETE,
	REQUEST_ACTIVE,
	REQUEST_OVERDUE
} MessageType_t;

typedef struct Scheduler_request_message{

	MessageType_t MessageType;
	QueueHandle_t ReplyQueue;

}Scheduler_request_message;

typedef struct Task_create_message{

	MessageType_t MessageType;
	TickType_t Deadline;
	QueueHandle_t ReplyQueue;
	TaskHandle_t TaskHandle;

}Task_create_message;

typedef struct Task_create_response{

	MessageType_t MessageType;
	TaskHandle_t TaskHandle;

}Task_create_response;

typedef struct Task_delete_message{

	MessageType_t MessageType;
	TaskHandle_t TaskHandle;
	QueueHandle_t ReplyQueue;

}Task_delete_message;

typedef struct Task_delete_response{

	MessageType_t MessageType;
	uint32_t retval;

}Task_delete_response;

typedef struct Task_request_response{

	MessageType_t MessageType;
	TaskList* List;

}Task_request_response;
typedef struct ID_message{

	MessageType_t MessageType;

}ID_message;

typedef union DD_Message{

	Scheduler_request_message RequestMessage;
	Task_create_message CreateMessage;
	Task_delete_message DeleteMessage;
	Task_create_response CreateResponse;
	Task_delete_response DeleteResponse;
	Task_request_response TaskListResponse;
	ID_message ID;

}DD_message;

/*-----------------------------------------------------------*/
// Global scheduler queue
xQueueHandle SchedulerQueue;
xQueueHandle ReplyQueue;




/*-----------------------------------------------------------*/
TaskHandle_t dd_tcreate(Task_param_s);
uint32_t dd_delete(TaskHandle_t);
uint32_t dd_return_active_list(const TaskList*);
uint32_t dd_return_overdue_list(const TaskList*);

/*-----------------------------------------------------------*/

#endif /* DD_SCHEDULER_H_ */
