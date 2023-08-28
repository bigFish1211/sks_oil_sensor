/*
 * drv_timer.c
 *
 *  Created on: Aug 28, 2023
 *      Author: thangnq
 */


#include "stm32g031xx.h"
#include "drv_timer.h"

static int timeInit(void){

	return 1;
}

static int TIM16_basic_int(void){
	//rcc int
	APBENR2 |= RCC_APBENR2_TIM16EN;
	//set clock devision
	TIM16->CCR1 &= ~TIM_CR1_CKD;
	return 1;
}
