/*
 * global.h
 *
 *  Created on: Aug 19, 2023
 *      Author: icepe
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "stdint.h"

#define COM_X0_RX_SIZE			128
#define MAX_BUFF_SIZE			128

extern uint8_t com_x0_rx_buf[COM_X0_RX_SIZE];
extern uint8_t buffer[MAX_BUFF_SIZE];
extern uint16_t buferLen;


extern volatile uint32_t ulMiliCount;
extern volatile uint32_t ulSecCount;
extern uint8_t com0_getData;
#endif /* GLOBAL_H_ */
