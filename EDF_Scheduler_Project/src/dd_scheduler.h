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

typedef struct Task_list_s{

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



TaskHandle_t dd_tcreate(Task_param_s);
uint32_t dd_delete(TaskHandle_t);
uint32_t dd_return_active_list(Task_list_s**);
uint32_t dd_return_overdue_list(Task_list_s**);


