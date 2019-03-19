/*
 * dd_scheduler.c
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */


#include "dd_scheduler.h"

/*-----------------------------------------------------------*/



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
//	QueueHandle_t ReplyQueue;
	TaskHandle_t TaskHandle;
	BaseType_t Returned;
	DD_message CreateResponse;
	DD_message CreateMessage;
	extern QueueHandle_t SchedulerQueue;

	// Create task
	Returned = xTaskCreate( param.task, param.name, configMINIMAL_STACK_SIZE, NULL, 0, &TaskHandle);

	if ( Returned != pdPASS ) {
		// Something went wrong with task creation
		return NULL;
	}

	// Create message struct
	CreateMessage.CreateMessage.MessageType = CREATE;
	CreateMessage.CreateMessage.Deadline = param.deadline;
	CreateMessage.CreateMessage.TaskHandle = param.task;

	// Send message struct to scheduler
	xQueueSend( SchedulerQueue, &CreateMessage, 0 );

	// Wait for reply @ queue
	if ( xQueueReceive( MessageQueue, &CreateResponse, 0 ) ) {
		// Received message, delete queue
//		vQueueDelete( ReplyQueue );
	}

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
	QueueHandle_t ReplyQueue;
	struct AMessage *pxMessage;
	Task_create_message *DeleteMessage;

	// Create queue
	ReplyQueue = xQueueCreate( 10, sizeof( unsigned long ) );

	// Delete task
	vTaskDelete( TaskHandle );

	// Create message struct
	DeleteMessage->MessageType = DELETE;
	DeleteMessage->ReplyQueue = ReplyQueue;
	DeleteMessage->TaskHandle = TaskHandle;

	// Send message struct to scheduler
	xQueueSend( SchedulerQueue, &DeleteMessage, (TickType_t) 100 );

	// Wait for reply @ queue
	if ( xQueueReceive( ReplyQueue, &(pxMessage), (TickType_t ) 100 ) ) {
		// Received message, delete queue
		vQueueDelete( ReplyQueue );
	}

	return DELETE;
}

uint32_t dd_return_active_list(Task_list_s *list){
	/*
	 *	open a queue
	 *	create task list request message
	 *	wait for reply
	 *	take reply
	 *	destroy q
	 *	return
	 */
	return 0;
}

uint32_t dd_return_overdue_list(Task_list_s *list){
	/*
	 *	open a queue
	 *	create task list request message
	 *	wait for reply
	 *	take reply
	 *	destroy q
	 *	return
	 */
	return 0;
}

