/*
 * dd_callback.c
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */

#include "dd_callback.h"

void vPeriodicCallback(void* arg)
{
	(void) arg;

	// Tell generator to add periodic tasks
	xEventGroupSetBits(xTimerEvents, (1 << 0));

	return;
}

void vDebounce(void* arg)
{
	(void) arg;

	static int debounce = 0;
	static int state = 0;
	if (STM_EVAL_PBGetState(BUTTON_USER))
	{

		if (debounce && state == 0)
		{
			xEventGroupSetBits(xTimerEvents, (1 << 1));
			state = 1;
		}
		debounce++;
	}
	else
	{
		debounce = 0;
		state = 0;
	}
}

void vMissedDeadline(void* arg)
{
	DD_message Message;

	Message.ID.MessageType = UPDATE_ACTIVE;

	// Send message to scheduler
	xQueueSend( SchedulerQueue, &Message, 0 );
}
void vListCallback(void* arg)
{
	// Tell monitor to ask for list
	xEventGroupSetBits(xMonitorEvents, (1 << 0));
}
