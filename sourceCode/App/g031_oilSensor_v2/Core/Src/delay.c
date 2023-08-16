/*
 * delay.c
 *
 *  Created on: Feb 14, 2017
 *      Author: ThangNguyen
 */
#include <stdint.h>
#include "delay.h"
#include "global.h"
#include "config.h"
#include "stm32g031xx.h"

void delay_us(unsigned int time_us) {
	unsigned int i;
	unsigned char j;
	for (i = 0; i < time_us; i++) {
		for (j = 0; j < 7; j++)
			;
	}
}
void delay_ms(unsigned int time_ms) {
	uint32_t next_time = ulMiliCount+ time_ms;
	while(ulMiliCount<next_time);
}

uint32_t getMicros(void) {
	uint32_t micros;
	micros = ulMiliCount * 1000 + 1000 - SysTick->VAL / 64;

	return micros;
}
