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
	extern QueueHandle_t SingleReplyQueue;
	extern QueueHandle_t SchedulerQueue;

	TaskHandle_t TaskHandle;
	BaseType_t Returned;
	DD_message Message;
	if(xSemaphoreTake(xReplyMutex, portMAX_DELAY) == pdTRUE)
	{
		if (SingleReplyQueue == NULL)
		{
			SingleReplyQueue = xQueueCreate(1,sizeof(DD_message));
			vQueueAddToRegistry( SingleReplyQueue, "SingleReplyQueue" );
		}
		else
		{
			printf("Shouldn't be here");
		}

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
		Message.CreateMessage.ReplyQueue = SingleReplyQueue;


		// Send message struct to scheduler
		if (xQueueSend( SchedulerQueue, &Message, portMAX_DELAY ) != pdTRUE) DPRINTF("Scheduler Queue Error\n") ;
		// Wait for reply @ queue
		while ( xQueueReceive( SingleReplyQueue, &Message, portMAX_DELAY ) != pdTRUE) {;}

		// Received message, delete queue
		vQueueDelete( SingleReplyQueue );
		SingleReplyQueue = NULL;
		xSemaphoreGive(xReplyMutex);
	}

	return TaskHandle;

}

void dd_delete(TaskHandle_t TaskHandle)
{
	extern QueueHandle_t SingleReplyQueue;
	extern QueueHandle_t SchedulerQueue;
	DD_message Message;

	if(xSemaphoreTake(xReplyMutex, portMAX_DELAY) == pdTRUE)
	{
		// Create queue
		if (SingleReplyQueue == NULL)
		{
			SingleReplyQueue = xQueueCreate(1,sizeof(DD_message));
			vQueueAddToRegistry( SingleReplyQueue, "DeleteReplyQueue" );
		}
		else
		{
			printf("Shouldn't be here");
		}

		// Delete task


		// Create message struct
		Message.DeleteMessage.MessageType = DELETE;
		Message.DeleteMessage.ReplyQueue = SingleReplyQueue;
		Message.DeleteMessage.TaskHandle = TaskHandle;

		// Send message struct to scheduler


		// Send message struct to scheduler
		if (xQueueSend( SchedulerQueue, &Message, portMAX_DELAY ) != pdTRUE) DPRINTF("Scheduler Queue Error\n") ;

		// Wait for reply @ queue
		while ( xQueueReceive( SingleReplyQueue, &Message, portMAX_DELAY ) != pdTRUE) {;}

		// Received message, delete queue
		vQueueDelete( SingleReplyQueue );
		SingleReplyQueue = NULL;
		xSemaphoreGive(xReplyMutex);
	}
	vTaskDelete(NULL);
	return;
}

uint32_t dd_return_active_list(const TaskList * list)
{
	extern QueueHandle_t SingleReplyQueue;
	extern QueueHandle_t SchedulerQueue;
	DD_message Message;
	char retarray[100];

	if(xSemaphoreTake(xReplyMutex, portMAX_DELAY) == pdTRUE)
	{
		// Create queue
		if (SingleReplyQueue == NULL)
		{
			SingleReplyQueue = xQueueCreate(1,sizeof(DD_message));
			vQueueAddToRegistry( SingleReplyQueue, "ActiveReplyQueue" );
		}
		else
		{
			printf("Shouldn't be here");
		}

		// Delete task


		// Create message struct
		Message.RequestMessage.MessageType = REQUEST_ACTIVE;
		Message.RequestMessage.ReplyQueue = SingleReplyQueue;

		// Send message struct to scheduler


		// Send message struct to scheduler
		if (xQueueSend( SchedulerQueue, &Message, portMAX_DELAY ) != pdTRUE) DPRINTF("Scheduler Queue Error\n") ;

		// Wait for reply @ queue
		while ( xQueueReceive( SingleReplyQueue, &Message, portMAX_DELAY ) != pdTRUE) {;}

		list = Message.TaskListResponse.List;
		// Received message, delete queue
		vQueueDelete( SingleReplyQueue );
		SingleReplyQueue = NULL;
		xSemaphoreGive(xReplyMutex);
	}
	return 0;
}

uint32_t dd_return_overdue_list(const TaskList *list)
{
	extern QueueHandle_t ReplyQueue;
	extern QueueHandle_t SchedulerQueue;
	DD_message Message;

	// Create queue
	if (ReplyQueue == NULL)
	{
		ReplyQueue = xQueueCreate(1,sizeof(DD_message));
	}
	else
	{
		printf("Shouldn't be here");
		return 1;
	}

	// Create message struct
	Message.RequestMessage.MessageType = REQUEST_OVERDUE;
	Message.RequestMessage.ReplyQueue = ReplyQueue;

	// Send message struct to scheduler
	xQueueSend( SchedulerQueue, &Message, 1000 );

	// Wait for reply @ queue
	while ( xQueueReceive( ReplyQueue, &Message, 0 ) != pdTRUE) {;}
		// Received message, delete queue
	vQueueDelete( ReplyQueue );
	ReplyQueue = NULL;
	list = Message.TaskListResponse.List;

	return 0;
}

