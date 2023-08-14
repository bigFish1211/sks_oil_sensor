/*
 * uart.h
 *
 *  Created on: Jan 21, 2021
 *      Author: Thangnguyen
 */

#ifndef UART_H_
#define UART_H_

#include "stdint.h"

void PC_uartInit(uint32_t baud);
void PC_uart_putChar(uint8_t data);

#endif /* UART_H_ */
