/*
 * dd_scheduler.c
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */

/* Includes */
#include "dd_scheduler.h"

/*-----------------------------------------------------------*/

TaskHandle_t dd_tcreate(Task_param_s param)
{
	extern QueueHandle_t SchedulerQueue;

	TaskHandle_t TaskHandle;
	BaseType_t Returned;
	DD_message Message;

	// Create task
	Returned = xTaskCreate( param.task, param.name, configMINIMAL_STACK_SIZE,(TickType_t) param.deadline, 0, &TaskHandle);

	if ( Returned != pdPASS ) {
		// Something went wrong with task creation
		return NULL;
	}

	// Create message struct
	Message.CreateMessage.MessageType = CREATE;
	Message.CreateMessage.Deadline = param.deadlinetick;
	Message.CreateMessage.TaskHandle = TaskHandle;
	Message.CreateMessage.OwnerHandle = xTaskGetCurrentTaskHandle();

	// Send message struct to scheduler
	if (xQueueSend( SchedulerQueue, &Message, portMAX_DELAY ) != pdTRUE) DPRINTF("Scheduler Queue Error\n") ;

	// Wait for reply
	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

	return TaskHandle;

}

void dd_delete(TaskHandle_t TaskHandle)
{
	extern QueueHandle_t SchedulerQueue;
	DD_message Message;

	// Create message struct
	Message.DeleteMessage.MessageType = DELETE;
	Message.DeleteMessage.TaskHandle = TaskHandle;
	Message.DeleteMessage.OwnerHandle = xTaskGetCurrentTaskHandle();

	// Send message struct to scheduler
	if (xQueueSend( SchedulerQueue, &Message, portMAX_DELAY ) != pdTRUE) DPRINTF("Scheduler Queue Error\n") ;

	// Wait for reply
	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

	// Received message, delete queue
	vTaskDelete(NULL);
	return;
}

uint32_t dd_return_active_list(const TaskList *list)
{
	extern QueueHandle_t ListQueue;
	extern QueueHandle_t SchedulerQueue;
	DD_message Message;

	// Create message struct
	Message.RequestMessage.MessageType = REQUEST_ACTIVE;

	// Send message struct to scheduler
	if (xQueueSend( SchedulerQueue, &Message, portMAX_DELAY ) != pdTRUE) DPRINTF("Scheduler Queue Error\n") ;

	// Wait for reply @ queue
	while ( xQueueReceive( ListQueue, &list, portMAX_DELAY ) != pdTRUE) {;}

	return 0;
}

uint32_t dd_return_overdue_list(const TaskList *list)
{
	extern QueueHandle_t ListQueue;
	extern QueueHandle_t SchedulerQueue;
	DD_message Message;

	// Create message struct
	Message.RequestMessage.MessageType = REQUEST_OVERDUE;

	// Send message struct to scheduler
	if (xQueueSend( SchedulerQueue, &Message, portMAX_DELAY ) != pdTRUE) DPRINTF("Scheduler Queue Error\n") ;

	// Wait for reply @ queue
	while ( xQueueReceive( ListQueue, &list, portMAX_DELAY ) != pdTRUE) {;}

	return 0;
}

