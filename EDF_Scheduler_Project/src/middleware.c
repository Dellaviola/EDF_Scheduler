/*
 * middleware.c
 *
 *  Created on: Mar 20, 2019
 *      Author: mdellavi
 */


#include "middleware.h"



void DISCO_LED_INIT()
{
	STM_EVAL_LEDInit(amber_led);
	STM_EVAL_LEDInit(green_led);
	STM_EVAL_LEDInit(red_led);
	STM_EVAL_LEDInit(blue_led);
	return;
}

void DISCO_BUTTON_INIT()
{
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
}

