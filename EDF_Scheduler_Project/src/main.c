/*
 * main.c
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */

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

/*-----------------------------------------------------------*/
#define mainQUEUE_LENGTH 100

#define amber  	0
#define green  	1
#define red  	2
#define blue  	3

#define amber_led	LED3
#define green_led	LED4
#define red_led		LED5
#define blue_led	LED6


/*
 * The queue send and receive tasks as described in the comments at the top of
 * this file.
 */
static void prvSetupHardware( void );
static void Manager_Task( void *pvParameters );
static void Blue_LED_Controller_Task( void *pvParameters );
static void Green_LED_Controller_Task( void *pvParameters );
static void Red_LED_Controller_Task( void *pvParameters );
static void Amber_LED_Controller_Task( void *pvParameters );

xQueueHandle xQueue_handle = 0;


/*-----------------------------------------------------------*/

int main(void)
{

	/* Initialize LEDs */
	STM_EVAL_LEDInit(amber_led);
	STM_EVAL_LEDInit(green_led);
	STM_EVAL_LEDInit(red_led);
	STM_EVAL_LEDInit(blue_led);

	/* Configure the system ready to run the demo.  The clock configuration
	can be done here if it was not done before main() was called. */
	prvSetupHardware();


	/* Create the queue used by the queue send and queue receive tasks.
	http://www.freertos.org/a00116.html */
	xQueue_handle = xQueueCreate( 	mainQUEUE_LENGTH,		/* The number of items the queue can hold. */
							sizeof( uint32_t ) );	/* The size of each item the queue holds. */

	/* Add to the registry, for the benefit of kernel aware debugging. */
	vQueueAddToRegistry( xQueue_handle, "MainQueue" );

	xTaskCreate( Manager_Task, "Manager", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate( Blue_LED_Controller_Task, "Blue_LED", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate( Red_LED_Controller_Task, "Red_LED", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate( Green_LED_Controller_Task, "Green_LED", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate( Amber_LED_Controller_Task, "Amber_LED", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	return 0;
}


/*-----------------------------------------------------------*/

static void Manager_Task( void *pvParameters )
{
	uint32_t tx_data = amber;


	while(1)
	{

		if(tx_data == amber)
			STM_EVAL_LEDOn(amber_led);
		if(tx_data == green)
			STM_EVAL_LEDOn(green_led);
		if(tx_data == red)
			STM_EVAL_LEDOn(red_led);
		if(tx_data == blue)
			STM_EVAL_LEDOn(blue_led);

		if( xQueueSend(xQueue_handle,&tx_data,1000))
		{
			printf("LED %lu ON!\n", tx_data);
			if(++tx_data == 4)
				tx_data = 0;
			vTaskDelay(1000);
		}
		else
		{
			printf("Manager Failed!\n");
		}
	}
}

/*-----------------------------------------------------------*/

static void Blue_LED_Controller_Task( void *pvParameters )
{
	uint32_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 1000))
		{
			if(rx_data == blue)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(blue_led);
				printf("Blue Off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,500))
					{
						printf("Blue GRP (%lu).\n", rx_data); // Got Wrong Package
						vTaskDelay(500);
					}
			}
		}
	}
}


/*-----------------------------------------------------------*/

static void Green_LED_Controller_Task( void *pvParameters )
{
	uint32_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 500))
		{
			if(rx_data == green)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(green_led);
				printf("Green Off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,1000))
					{
						printf("Green GRP (%lu).\n", rx_data); // Got Wrong Package
						vTaskDelay(500);
					}
			}
		}
	}
}

/*-----------------------------------------------------------*/

static void Red_LED_Controller_Task( void *pvParameters )
{
	uint32_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 500))
		{
			if(rx_data == red)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(red_led);
				printf("Red off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,1000))
					{
						printf("Red GRP (%lu).\n", rx_data); // Got Wrong Package
						vTaskDelay(500);
					}
			}
		}
	}
}


/*-----------------------------------------------------------*/

static void Amber_LED_Controller_Task( void *pvParameters )
{
	uint32_t rx_data;
	while(1)
	{
		if(xQueueReceive(xQueue_handle, &rx_data, 500))
		{
			if(rx_data == amber)
			{
				vTaskDelay(250);
				STM_EVAL_LEDOff(amber_led);
				printf("Amber Off.\n");
			}
			else
			{
				if( xQueueSend(xQueue_handle,&rx_data,1000))
					{
						printf("Amber GRP (%lu).\n", rx_data); // Got Wrong Package
						vTaskDelay(500);
					}
			}
		}
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
