/*
 * drv_io.c
 *
 *  Created on: Apr 3, 2023
 *      Author: icepe
 */


#include "drv_io.h"
#include "stm32g0xx.h"


void gpio_clock_init(void){
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
	RCC->IOPENR |= RCC_IOPENR_GPIODEN;
	RCC->IOPENR |= RCC_IOPENR_GPIOFEN;
}

void gpio_pin_int(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, uint8_t gpio_pinMode){
	if(gpio_pin_outPut == gpio_pinMode){
		gpio_port->MODER &= ~(0x03<<(gpio_pin*2));
		gpio_port->MODER |= (0x01<<(gpio_pin*2));
	}
	else if(gpio_pin_inPut == gpio_pinMode){
		gpio_port->MODER &= ~(0x03<<(gpio_pin*2));

	}
}

typedef enum {
	TRIGGER_RISING = 0x01,
	TRIGGER_FALLING = 0x02,
}EXTI_INTERRUP_MODE;

void GpioInterruptInit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){

	uint32_t position = 0x00u;
	uint32_t iocurrent;
	uint32_t temp;
	//mode input
	gpio_port->MODER &= ~(0x03<<(gpio_pin*2));

	temp = EXTI->EXTICR[position >> 2u];
	temp &= ~(0x0FuL << (8u * (position & 0x03u)));
	temp |= (GPIO_GET_INDEX(GPIOx) << (8u * (position & 0x03u)));
	EXTI->EXTICR[position >> 2u] = temp;

	/* Clear Rising Falling edge configuration */
	temp = EXTI->RTSR1;
	temp &= ~(iocurrent);
	if ((GPIO_Init->Mode & TRIGGER_RISING) != 0x00u) {
		temp |= iocurrent;
	}
	EXTI->RTSR1 = temp;

	temp = EXTI->FTSR1;
	temp &= ~(iocurrent);
	if ((GPIO_Init->Mode & TRIGGER_FALLING) != 0x00u) {
		temp |= iocurrent;
	}
	EXTI->FTSR1 = temp;

	/* Clear EXTI line configuration */
	temp = EXTI->EMR1;
	temp &= ~(iocurrent);
	if ((GPIO_Init->Mode & EXTI_EVT) != 0x00u) {
		temp |= iocurrent;
	}
	EXTI->EMR1 = temp;

	temp = EXTI->IMR1;
	temp &= ~(iocurrent);
	if ((GPIO_Init->Mode & EXTI_IT) != 0x00u) {
		temp |= iocurrent;
	}
	EXTI->IMR1 = temp;
}

void gpio_write(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, uint8_t gpio_pinState){
	if(gpio_pin_reset == gpio_pinState){
		gpio_port->ODR &= ~(1<<gpio_pin);
	}
	else
//		if(gpio_pin_set == gpio_pinState)
		{
		gpio_port->ODR |= 1<<gpio_pin;
	}
}

uint8_t gpio_read(GPIO_TypeDef *gpio_port, uint16_t gpio_pin){
	if(0x01==((gpio_port->IDR>>gpio_pin)&0x01))
		return 1;
	else
	return 0;
}


