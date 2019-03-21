/*
 * middleware.h
 *
 *  Created on: Mar 20, 2019
 *      Author: mdellavi
 */

#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../Libraries/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h"


#define amber  	0
#define green  	1
#define red  	2
#define blue  	3

#define amber_led	LED3
#define green_led	LED4
#define red_led		LED5
#define blue_led	LED6

void DISCO_LED_INIT(void);
void DISCO_BUTTON_INIT(void);
#endif /* MIDDLEWARE_H_ */
