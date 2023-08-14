/*
 * global.h
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdbool.h>
#include "main.h"
#include "define.h"

typedef struct _APP_STATS {
	volatile uint32_t	tickCount;
	volatile uint32_t 	miliCount;
	volatile uint32_t	secCount;
	volatile uint32_t	lastRaise;
	uint8_t 			saveConfig;
	uint8_t				pageCount;
	uint32_t			imgSize;
	uint8_t 			jump_application;
} APP_STATS;

typedef struct {
	/*UART_HandleTypeDef *uartPort;
	DMA_HandleTypeDef *dmaPort;*/
	uint8_t rxBuff[RX_BUF_LEN];
	uint16_t idx;
	volatile uint32_t lastRaise;
	uint16_t rxLen;
} COMM_ATTRIBUTE;

typedef struct {
	uint32_t key;
	uint32_t len;
	uint8_t available;

}T_FIRMWARE;

extern COMM_ATTRIBUTE pc_comm;
extern APP_STATS g_appStats;
extern T_FIRMWARE appFirmWare;
#endif /* INC_GLOBAL_H_ */
