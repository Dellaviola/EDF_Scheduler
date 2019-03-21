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
#include "options.h"


/*-----------------------------------------------------------*/

// Task Prototypes
static void prvSetupHardware( void );
static void DD_Scheduler_Task( void *pvParameters );
static void DD_Generator_Task( void *pvParameters );
static void DD_Monitor_Task( void *pvParameters );
//static void vInterrupt_Handler_Task(void *pvParameters);

//static TaskHandle_t Interrupt;

/*-----------------------------------------------------------*/

// User Task Space
static void User_Periodic( void * pvParameters )
{
	static uint32_t led = 0;
	led++;
	if (led == 4) led = 0;
	uint32_t ledmemory = led;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		STM_EVAL_LEDOn(ledmemory);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS((TickType_t) pvParameters));
		TaskHandle_t x = xTaskGetCurrentTaskHandle();
		STM_EVAL_LEDOff(ledmemory);
		dd_delete(x);
	}
}
static void User_Random( void *pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	static uint16_t count = 0;
	while (1)
	{
		printf("%u: Push Button Pressed!\n", count++);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS((TickType_t) pvParameters));
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
	DISCO_BUTTON_INIT();

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

	// Create Mutex
//	xReplyMutex = xSemaphoreCreateMutex();
//	xSemaphoreGive(xReplyMutex);
//	vQueueAddToRegistry( xReplyMutex, "Reply Mutex" );

	// Create Timers
	xTimers[0] = xTimerCreate("PeriodicTask", pdMS_TO_TICKS(2000), pdFALSE, (void *) 0, vPeriodicCallback);
	xTimers[1] = xTimerCreate("Debounce", pdMS_TO_TICKS(50), pdTRUE, (void *) 0, vDebounce);

	// Create Tasks
	xTaskCreate( DD_Scheduler_Task, "Scheduler", configMINIMAL_STACK_SIZE, &container, 30, NULL);
	xTaskCreate( DD_Generator_Task, "Generator", configMINIMAL_STACK_SIZE, NULL, 29, NULL);
	xTaskCreate( DD_Monitor_Task, "Monitor", configMINIMAL_STACK_SIZE, &container, 0, NULL);
	//xTaskCreate( vInterrupt_Handler_Task, "Interrupt", configMINIMAL_STACK_SIZE, NULL, 31, &Interrupt);


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
		if ( xQueueReceive( SchedulerQueue, &Received, 1000 ) ) {

			DPRINTF("Task Message: %d\n", Received.ID.MessageType);

			switch(Received.ID.MessageType)
			{
				case(CREATE):
					DPRINTF("Acknowledge Create Request\n");

					// Add it to Active List
					list_add(param->Active, Received.CreateMessage.TaskHandle, Received.CreateMessage.Deadline);

					Out.CreateResponse.MessageType = CREATE;
					Out.CreateResponse.TaskHandle = Received.CreateMessage.TaskHandle;

					xQueueSend(Received.CreateMessage.ReplyQueue, &Out, 1000);

					break;

				case(DELETE):
					DPRINTF("Acknowledge Delete Request\n");

					// Remove it from active list
					list_remove(param->Active, Received.DeleteMessage.TaskHandle);

					Out.DeleteResponse.MessageType = CREATE;
					Out.DeleteResponse.retval = DELETE;

					xQueueSend(Received.DeleteMessage.ReplyQueue, &Out, 1000);

					break;

				case(REQUEST_ACTIVE):
					DPRINTF("Acknowledge Active List Request\n");
					Out.TaskListResponse.MessageType = REQUEST_ACTIVE;
					Out.TaskListResponse.List = param->Active;
					xQueueSend(Received.RequestMessage.ReplyQueue, &Out, 1000);
				break;

				case(REQUEST_OVERDUE):
					DPRINTF("Acknowledge Overdue List Request\n");
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
	strcpy(PeriodicTaskParam.name, "Periodic Instance");

	Task_param_s RandomTaskParam;

	RandomTaskParam.task = User_Random;
	strcpy(RandomTaskParam.name, "Random Instance");

	xTimerEvents = xEventGroupCreate();
	EventBits_t EventBits;
	if (xTimerEvents == NULL)
	{

	}
	else
	{
		xTimerStart(xTimers[0], 0);
		xTimerStart(xTimers[1], 0);
	}

	while(1)
	{

		EventBits = xEventGroupWaitBits(xTimerEvents, (1 << 0) | (1 << 1), pdTRUE, pdFALSE, pdMS_TO_TICKS(10000));
		if ((EventBits & (1 << 0)) == (1 << 0))
		{
			PeriodicTaskParam.deadlinetick = (xTaskGetTickCount() + PeriodicTaskParam.deadline);
			dd_tcreate(PeriodicTaskParam);
			xTimerStart(xTimers[0],0);
		}
		else if ((EventBits & (1 << 1)) == (1 << 1))
		{
			RandomTaskParam.deadline = pdMS_TO_TICKS((rand() % 5000) + 5000);
			RandomTaskParam.deadlinetick = (xTaskGetTickCount() + RandomTaskParam.deadline);
			dd_tcreate(RandomTaskParam);
		}
		else
		{
			DPRINTF("Generator unblocking for error checking\n");
		}

		//ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));

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

//void EXTI0_IRQHandler(void)
//{
//    // Checks whether the interrupt from EXTI0 or not
//    if (EXTI_GetITStatus(EXTI_Line0))
//    {
//
//    	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//    	vTaskNotifyGiveFromISR(Interrupt, &xHigherPriorityTaskWoken);
//
//    	// Clears the EXTI line pending bit
//        EXTI_ClearITPendingBit(EXTI_Line0);
//        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//    }
//}
//
//static void vInterrupt_Handler_Task(void *pvParameters)
//{
//
//	uint32_t ulTaskNotification = 0;
//
//		while(1)
//		{
//			ulTaskNotification = ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(5000));
//			vTaskDelay(pdMS_TO_TICKS(50));
//			if (ulTaskNotification == 0){
//				DPRINTF("ISR CHECK\n");
//			}
//			else if (STM_EVAL_PBGetState(BUTTON_USER))
//			{
//				xEventGroupSetBits(xTimerEvents, (1 << 1));
//			}
//			else
//			{
//				DPRINTF("Spurious Interrupt");
//			}
//		}
//
//}

/*-----------------------------------------------------------*/

// Necessary for linker
static void prvSetupHardware( void )
{
	NVIC_SetPriorityGrouping( 0 );
}
