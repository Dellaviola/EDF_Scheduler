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
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"
/* Includes */
#include "rtos_hooks.h"
#include "dd_scheduler.h"
#include "string.h"

/*-----------------------------------------------------------*/

static void prvSetupHardware( void );
static void DD_Scheduler_Task( void *pvParameters );
static void DD_Generator_Task( void *pvParameters );
static void DD_Monitor_Task( void *pvParameters );

/*-----------------------------------------------------------*/

struct MessageBuffer {
	int MessageType;
} MessageBuffer;

/*-----------------------------------------------------------*/
static void DD_User_Task1( void *pvParameters )
{
	while (1)
	{
		printf("User Task 1\n");
		vTaskDelay(1000);
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


	// Create Tasks
	xTaskCreate( DD_Scheduler_Task, "Scheduler", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate( DD_Generator_Task, "Generator", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
//	xTaskCreate( DD_Monitor_Task, "Monitor", configMINIMAL_STACK_SIZE, NULL, 1, NULL);


	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	return 0;
}


/*-----------------------------------------------------------*/

static void DD_Scheduler_Task( void *pvParameters )
{
	DD_message Received;
	DD_message Out;

	while(1)
	{

		if ( xQueueReceive( SchedulerQueue, &Received, (TickType_t) 0 ) ) {

			printf("Task Message: %d\n", Received.ID.MessageType);

			switch(Received.ID.MessageType)
			{
				case(CREATE):
					printf("Acknowledge Create Request\n");
					Out.CreateResponse.MessageType = CREATE;
					Out.CreateResponse.TaskHandle = Received.CreateMessage.TaskHandle;
					xQueueSend(Received.CreateMessage.ReplyQueue, &Out, 1000);
				break;

				case(DELETE):
					printf("Acknowledge Delete Request\n");
					Out.DeleteResponse.MessageType = CREATE;
					Out.DeleteResponse.retval = DELETE;
					xQueueSend(Received.DeleteMessage.ReplyQueue, &Out, 1000);
				break;

				case(REQUEST_ACTIVE):
					//wad
				break;

				case(REQUEST_OVERDUE):
					//wad
				break;

				default:

				break;
			}
		}


//		printf("HELLO WORLD");
	}
}

/*-----------------------------------------------------------*/

static void DD_Generator_Task( void *pvParameters )
{
	Task_param_s TaskParam;

	while(1)
	{
		TaskParam.task = DD_User_Task1;
		TaskParam.deadline = 100;
		TaskParam.execution = 200;
		strcpy(TaskParam.name,"user");
		TaskHandle_t x = dd_tcreate(TaskParam);
		vTaskDelay(pdMS_TO_TICKS(1000));
		dd_delete(x);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


/*-----------------------------------------------------------*/

static void DD_Monitor_Task( void *pvParameters )
{
	while(1)
	{

	}
}

/*-----------------------------------------------------------*/
// Necessary misc functions
static void prvSetupHardware( void )
{
	/* Ensure all priority bits are assigned as preemption priority bits.
	http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping( 0 );

	/* TODO: Setup the clocks, etc. here, if they were not configured before
	main() was called. */
}
