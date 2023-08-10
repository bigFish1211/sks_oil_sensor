/*
 * uart.h
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */


#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

void comm_init(void);

void PC_putChar(uint8_t ch);
void PC_putString(uint8_t *str);
void PC_putArray(uint8_t *str, int len);

#endif /* INC_GPIO_H_ */
