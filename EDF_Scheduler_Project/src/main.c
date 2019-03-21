/*
 * main.c
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */

/*-----------------------------------------------------------*/

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4_discovery.h"

/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"
#include "../FreeRTOS_Source/include/event_groups.h"

/* Includes */
#include "rtos_hooks.h"
#include "dd_scheduler.h"
#include "string.h"
#include "list.h"
#include "dd_callback.h"
#include "middleware.h"


/*-----------------------------------------------------------*/

// Task Prototypes
static void prvSetupHardware( void );
static void DD_Scheduler_Task( void *pvParameters );
static void DD_Generator_Task( void *pvParameters );
static void DD_Monitor_Task( void *pvParameters );

/*-----------------------------------------------------------*/

// User Task Space
static void User_Periodic( void * pvParameters )
{
	static uint32_t led = 0;
	led++;
	if (led == 4) led = 0;
	uint32_t ledmemory = led;
	(TickType_t) pvParameters;

	while (1)
	{
		STM_EVAL_LEDOn(ledmemory);
		vTaskDelay(pvParameters);
		TaskHandle_t x = xTaskGetCurrentTaskHandle();
		STM_EVAL_LEDOff(ledmemory);
		dd_delete(x);
	}
}
static void User_Random( void *pvParameters )
{
	while (1)
	{
		printf("User Task 1\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
		TaskHandle_t x = xTaskGetCurrentTaskHandle();
		dd_delete(x);
	}
}

/*-----------------------------------------------------------*/

int main(void)
{

	/* Configure the system ready to run the demo.  The clock configuration
	can be done here if it was not done before main() was called. */
	prvSetupHardware();

	// Init queue
	SchedulerQueue = xQueueCreate( 8, sizeof(DD_message) );
	vQueueAddToRegistry( SchedulerQueue, "SchedulerQueue" );

	// Init LEDS
	DISCO_LED_INIT();

	// Create Lists
	static TaskList *ActiveList;
	static TaskList *OverdueList;

	ActiveList = malloc(sizeof(TaskList *));
	OverdueList = malloc(sizeof(TaskList *));

	ActiveList->Next = NULL;
	OverdueList->Next = NULL;
	ActiveList->Handle = NULL;
	OverdueList->Handle = NULL;

	static ListContainer container;
	container.Active = ActiveList;
	container.Overdue = OverdueList;

	// Max List Size = 28

	// Create Timers
	xTimers[0] = xTimerCreate("PeriodicTask", pdMS_TO_TICKS(2000), pdFALSE, (void *) 0, vPeriodicCallback);

	// Create Tasks
	xTaskCreate( DD_Scheduler_Task, "Scheduler", configMINIMAL_STACK_SIZE, &container, 30, NULL);
	xTaskCreate( DD_Generator_Task, "Generator", configMINIMAL_STACK_SIZE, NULL, 29, NULL);
	xTaskCreate( DD_Monitor_Task, "Monitor", configMINIMAL_STACK_SIZE, &container, 0, NULL);


	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	return 0;
}

/*-----------------------------------------------------------*/

static void DD_Scheduler_Task( void *pvParameters )
{
	DD_message Received;
	DD_message Out;

	ListContainer* param = (ListContainer* )pvParameters;

	while(1)
	{
		if ( xQueueReceive( SchedulerQueue, &Received, (TickType_t) 10 ) ) {

			printf("Task Message: %d\n", Received.ID.MessageType);

			switch(Received.ID.MessageType)
			{
				case(CREATE):
					printf("Acknowledge Create Request\n");

					// Add it to Active List
					list_add(param->Active, Received.CreateMessage.TaskHandle, Received.CreateMessage.Deadline);
						// TODO: param active points to a freed space, need to move its pointer somehow.
					// Create outgoing reply
					Out.CreateResponse.MessageType = CREATE;
					Out.CreateResponse.TaskHandle = Received.CreateMessage.TaskHandle;
					xQueueSend(Received.CreateMessage.ReplyQueue, &Out, 1000);
				break;

				case(DELETE):
					printf("Acknowledge Delete Request\n");

					// Remove it from active list
					list_remove(param->Active, Received.DeleteMessage.TaskHandle);

					Out.DeleteResponse.MessageType = CREATE;
					Out.DeleteResponse.retval = DELETE;
					xQueueSend(Received.DeleteMessage.ReplyQueue, &Out, 1000);
				break;

				case(REQUEST_ACTIVE):
					printf("Acknowledge Active List Request\n");
					Out.TaskListResponse.MessageType = REQUEST_ACTIVE;
					Out.TaskListResponse.List = param->Active;
					xQueueSend(Received.RequestMessage.ReplyQueue, &Out, 1000);
				break;

				case(REQUEST_OVERDUE):
					printf("Acknowledge Overdue List Request\n");
					Out.TaskListResponse.MessageType = REQUEST_OVERDUE;
					Out.TaskListResponse.List = param->Overdue;
					xQueueSend(Received.RequestMessage.ReplyQueue, &Out, 1000);
				break;

				case(UPDATE_ACTIVE):
					printf("Acknowledge Update Active List Requests\n");
					// Received->
				break;

				default:
					printf("nicelydone");
				break;
			}

			// Update priorities
			UBaseType_t i;
			TaskList * temp = param->Active;
			for (i = list_size(temp) + 1; i > 1; i--) {
				if (temp->Handle) vTaskPrioritySet( temp->Handle, i );
				temp = temp->Next;
			}
		}
	}
}

/*-----------------------------------------------------------*/

static void DD_Generator_Task( void *pvParameters )
{
	Task_param_s PeriodicTaskParam;

	PeriodicTaskParam.task = User_Periodic;
	PeriodicTaskParam.deadline = pdMS_TO_TICKS(750);
	PeriodicTaskParam.deadlinetick = xTaskGetTickCount() + PeriodicTaskParam.deadline;
	strcpy(PeriodicTaskParam.name, "Periodic Instance");

	Task_param_s RandomTaskParam;

	RandomTaskParam.deadline = pdMS_TO_TICKS((rand() % 1000) + 100);

	xTimerEvents = xEventGroupCreate();
	EventBits_t EventBits;
	if (xTimerEvents == NULL)
	{

	}
	else
	{
		xTimerStart(xTimers[0], 0);
	}

	while(1)
	{

		EventBits = xEventGroupWaitBits(xTimerEvents, (1 << 0), pdTRUE, pdFALSE, pdMS_TO_TICKS(2000));
		if ((EventBits & (1 << 0)) == (1 << 0))
		{
			dd_tcreate(PeriodicTaskParam);
			xTimerStart(xTimers[0],0);
		}
		else
		{

		}

		ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));

	}
}


/*-----------------------------------------------------------*/

static void DD_Monitor_Task( void *pvParameters )
{
	ListContainer* param = (ListContainer* )pvParameters;

	while(1)
	{
		//printf("idling\n");

		// Message Scheduler to update the lists
//		xQueueSend(blah blah);

		// Check system usage, etc.
	}
}

/*-----------------------------------------------------------*/

// Necessary for linker
static void prvSetupHardware( void )
{
	NVIC_SetPriorityGrouping( 0 );
}
