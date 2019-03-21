/*
 * list.h
 *
 *  Created on: Mar 19, 2019
 *      Author: mdellavi
 */

#ifndef LIST_H_
#define LIST_H_

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

#include "options.h"

/*-----------------------------------------------------------*/

typedef struct TaskList{

	TaskHandle_t Handle;
	TickType_t Deadline;
	uint32_t TaskType;
	struct TaskList* Next;


}TaskList, TaskNode;

typedef struct ListContainer{
	TaskList *Active;
	TaskList *Overdue;
}ListContainer;

/*-----------------------------------------------------------*/

// List Prototypes
void list_init(void); //creates null list head
void list_add(TaskList *, TaskHandle_t, TickType_t);
void list_remove(TaskList *, TaskHandle_t);
void list_delete(TaskList *);

int list_size(TaskList *);

#endif /* LIST_H_ */
