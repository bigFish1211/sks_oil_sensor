/*
 * drv_io.h
 *
 *  Created on: Apr 3, 2023
 *      Author: icepe
 */

#ifndef INC_DRV_IO_H_
#define INC_DRV_IO_H_
#include <stdint.h>
#include "stm32g0xx.h"

typedef enum{
	gpio_pin_outPut = 0,
	gpio_pin_inPut = 1,
}t_gpio_pinMode;

typedef enum{
	gpio_pin_reset = 0,
	gpio_pin_set = 1,
}t_gpio_pinState;

void gpio_clock_init(void);
void gpio_pin_int(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, uint8_t gpio_pinMode);
void GpioInterruptInit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void gpio_write(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, uint8_t gpio_pinState);
uint8_t gpio_read(GPIO_TypeDef *gpio_port, uint16_t gpio_pin);
#endif /* INC_DRV_IO_H_ */
