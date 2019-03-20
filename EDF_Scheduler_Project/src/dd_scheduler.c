/*
 * dd_scheduler.c
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */


#include "dd_scheduler.h"

/*-----------------------------------------------------------*/

//hi

/*-----------------------------------------------------------*/

TaskHandle_t dd_tcreate(Task_param_s param){
	/*
	 * open a queue
	 * create min priority task give task params
	 * create a task message and send to scheduler
	 * wait for reply at q
	 * take reply
	 * destroy q
	 * return null or task handle
	 */
	// Variables
	extern QueueHandle_t ReplyQueue;
	extern QueueHandle_t SchedulerQueue;

	TaskHandle_t TaskHandle;
	BaseType_t Returned;
	DD_message Message;

	if (ReplyQueue == NULL)
	{
		ReplyQueue = xQueueCreate(1,sizeof(DD_message));
	}
	else
	{
		printf("Shouldn't be here");
	}

	vQueueAddToRegistry( ReplyQueue, "ReplyQueue" );
	// Create task
	Returned = xTaskCreate( param.task, param.name, configMINIMAL_STACK_SIZE, NULL, 0, &TaskHandle);

	if ( Returned != pdPASS ) {
		// Something went wrong with task creation
		return NULL;
	}

	// Create message struct
	Message.CreateMessage.MessageType = CREATE;
	Message.CreateMessage.Deadline = param.deadline;
	Message.CreateMessage.TaskHandle = param.task;
	Message.CreateMessage.ReplyQueue = ReplyQueue;

	// Send message struct to scheduler
	xQueueSend( SchedulerQueue, &Message, 1000 );

	// Wait for reply @ queue
	while ( xQueueReceive( ReplyQueue, &Message, 0 ) != pdTRUE) {;}
		// Received message, delete queue
	vQueueDelete( ReplyQueue );
	ReplyQueue = NULL;

	return TaskHandle;

}

uint32_t dd_delete(TaskHandle_t TaskHandle){
	/*
	 * open a queue
	 * delete task handle
	 * create task delete message send to scheduler
	 * wait for reply
	 * take reply
	 * destroy q
	 * return
	 */
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
	}

	// Delete task
	vTaskDelete( TaskHandle );

	// Create message struct
	Message.DeleteMessage.MessageType = DELETE;
	Message.DeleteMessage.ReplyQueue = ReplyQueue;
	Message.DeleteMessage.TaskHandle = TaskHandle;

	// Send message struct to scheduler
	xQueueSend( SchedulerQueue, &Message, 1000 );

	// Wait for reply @ queue
	while ( xQueueReceive( ReplyQueue, &Message, 0 ) != pdTRUE) {;}
		// Received message, delete queue
	vQueueDelete( ReplyQueue );
	ReplyQueue = NULL;
	printf("Deleted Task, returning %d\n", Message.DeleteResponse.retval);
	return Message.DeleteResponse.retval;
}

uint32_t dd_return_active_list(const TaskList *list){
	/*
	 *	open a queue
	 *	create task list request message
	 *	wait for reply
	 *	take reply
	 *	destroy q
	 *	return
	 */
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
	Message.RequestMessage.MessageType = REQUEST_ACTIVE;
	Message.RequestMessage.ReplyQueue = ReplyQueue;

	// Send message struct to scheduler
	xQueueSend( SchedulerQueue, &Message, 1000 );

	// Wait for reply @ queue
	while ( xQueueReceive( ReplyQueue, &Message, 0 ) != pdTRUE) {;}
		// Received message, delete queue
	vQueueDelete( ReplyQueue );
	ReplyQueue = NULL;

	return 0;
}

uint32_t dd_return_overdue_list(const TaskList *list){
	/*
	 *	open a queue
	 *	create task list request message
	 *	wait for reply
	 *	take reply
	 *	destroy q
	 *	return
	 */
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

	return 0;
}

