/*
 * uart.h
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */


#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h>

typedef enum{
	PIN_AF_0 = 0,
	PIN_AF_1 = 1,
	PIN_AF_2 = 2,
	PIN_AF_3 = 3,
	PIN_AF_4 = 4,
	PIN_AF_5 = 5,
	PIN_AF_6 = 6,
	PIN_AF_7 = 7,
	PIN_AF_8 = 8
}PIN_AF;

void usart_x0_init(uint32_t baud);
void usart_x0_sendChar(uint8_t c);

#endif /* INC_GPIO_H_ */
