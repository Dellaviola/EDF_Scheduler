/*
 * dd_callback.h
 *
 *  Created on: Mar 20, 2019
 *      Author: mdellavi
 */

#ifndef DD_CALLBACK_H_
#define DD_CALLBACK_H_

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
#include "../FreeRTOS_Source/include/event_groups.h"

#include "options.h"
#include "dd_scheduler.h"

EventGroupHandle_t xTimerEvents;

void vPeriodicCallback(void* arg);
void vDebounce(void* arg);
void vMissedDeadline(void* arg);



#endif /* DD_CALLBACK_H_ */
