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
