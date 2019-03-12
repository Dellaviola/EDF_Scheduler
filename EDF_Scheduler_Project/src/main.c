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

/*-----------------------------------------------------------*/

static void prvSetupHardware( void );
static void DD_Scheduler_Task( void *pvParameters );
static void DD_Generator_Task( void *pvParameters );
static void DD_Monitor_Task( void *pvParameters );

/*-----------------------------------------------------------*/

int main(void)
{

	/* Configure the system ready to run the demo.  The clock configuration
	can be done here if it was not done before main() was called. */
	prvSetupHardware();

	// Create Tasks
	xTaskCreate( DD_Scheduler_Task, "Scheduler", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate( DD_Generator_Task, "Generator", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate( DD_Monitor_Task, "Monitor", configMINIMAL_STACK_SIZE, NULL, 1, NULL);


	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	return 0;
}


/*-----------------------------------------------------------*/

static void DD_Scheduler_Task( void *pvParameters )
{

	while(1)
	{
		printf("HELLO WORLD");
	}
}

/*-----------------------------------------------------------*/

static void DD_Generator_Task( void *pvParameters )
{
	while(1)
	{

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
static void prvSetupHardware( void )
{
	/* Ensure all priority bits are assigned as preemption priority bits.
	http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping( 0 );

	/* TODO: Setup the clocks, etc. here, if they were not configured before
	main() was called. */
}
