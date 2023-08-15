/*
 * register_uart.h
 *
 *  Created on: May 4, 2023
 *      Author: KhanhNguyen
 */

#ifndef INC_REGISTER_UART_H_
#define INC_REGISTER_UART_H_
#include <stdint.h>
#include "stm32g031xx.h"
#include "main.h"
#include "gpio.h"

#define SYS_CLOCK 				64000000ul
#define COMn                             6
#define	REGISTER_UART1			((uart*)0x40013800)



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

typedef enum {
	uart_baud_9600 = 9600,
	uart_baud_19200 = 19200,
	uart_baud_38400 = 38400,
	uart_baud_57600 = 57600,
	uart_baud_115200 = 115200,
} UART_BAUDRATE;

typedef struct{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	volatile uint32_t GTPR;
	volatile uint32_t RTOR;
	volatile uint32_t RQR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t RDR;
	volatile uint32_t TDR;
	volatile uint32_t PRESC;
} uart;

static void register_uart1_int(uint32_t register_baud);

static void register_uart1_PutChar(uint8_t ch);
static void register_uart1_PutString(uint8_t* str);
static void register_uart1_putArray(uint8_t* str, int len);

void pc_phut(uint32_t bau);

#endif /* INC_REGISTER_UART_H_ */
