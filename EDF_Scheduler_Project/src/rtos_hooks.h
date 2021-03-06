/*
 * rtos_hooks.h
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */

#ifndef RTOS_HOOKS_H_
#define RTOS_HOOKS_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/task.h"

void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName );
void vApplicationIdleHook( void );



#endif /* RTOS_HOOKS_H_ */
