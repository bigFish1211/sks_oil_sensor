/*
 * dvr_uart.h
 *
 *  Created on: Jan 3, 2023
 *      Author: icepe
 */

#ifndef DVR_UART_H_
#define DVR_UART_H_
#include <stdio.h>


typedef enum{
	PIN_AF_0 = 0,
	PIN_AF_1 = 1,
	PIN_AF_2 = 2,
	PIN_AF_3 = 3,
	PIN_AF_4 = 4,
	PIN_AF_5 = 5,
	PIN_AF_6 = 6,
	PIN_AF_7 = 7,
	PIN_AF_8 = 8,
	PIN_AF_9 = 9,
	PIN_AF_10 = 10,
	PIN_AF_11 = 11,
	PIN_AF_12 = 12,
	PIN_AF_13 = 13,
	PIN_AF_14 = 14,
	PIN_AF_15 = 15,
}PIN_AF;

void usart_x0_init(uint32_t baudrate);
void SerialPutChar(uint8_t c) ;
uint32_t SerialKeyPressed(uint8_t *key);
int32_t Rcv_Byte(uint8_t *c, uint32_t timeout);
int32_t RcvBuff(uint8_t *buff, int *length, uint32_t timeout);

#endif /* DVR_UART_H_ */
