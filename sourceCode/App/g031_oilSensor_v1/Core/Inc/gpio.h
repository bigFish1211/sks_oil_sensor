/*
 * gpio.h
 *
 *  Created on: May 5, 2023
 *      Author: KhanhNguyen
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

#define	REGISTER_GPIOA			((GPIO*)0x50000000)

typedef struct{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
} GPIO;

void MX_GPIO_Init(void);
#endif /* INC_GPIO_H_ */
