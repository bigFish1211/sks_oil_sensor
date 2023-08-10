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

void GpioInterruptInit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
}

void gpio_write(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, uint8_t gpio_pinState){
	if (gpio_pin_reset == gpio_pinState) {
		gpio_port->ODR &= ~(1 << gpio_pin);
	} else {
		gpio_port->ODR |= 1 << gpio_pin;
	}
}

uint8_t gpio_read(GPIO_TypeDef *gpio_port, uint16_t gpio_pin){
	if(0x01==((gpio_port->IDR>>gpio_pin)&0x01))
		return 1;
	else
	return 0;
}


