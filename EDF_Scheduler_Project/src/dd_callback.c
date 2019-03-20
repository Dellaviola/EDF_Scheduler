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

	xEventGroupSetBits(xTimerEvents, (1<<0));

	return;
}
