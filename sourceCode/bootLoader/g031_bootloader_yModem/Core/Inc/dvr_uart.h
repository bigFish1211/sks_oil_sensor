/*
 * uart.h
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */


#ifndef INC_DVR_UART_H_
#define INC_DVR_UART_H_

#include "main.h"

void comm_init(void);
uint32_t SerialKeyPressed(uint8_t *key);
int32_t Rcv_Byte(uint8_t *c, uint32_t timeout);
int32_t RcvBuff(uint8_t *buff, int *length, uint32_t timeout);

#endif /* INC_GPIO_H_ */
