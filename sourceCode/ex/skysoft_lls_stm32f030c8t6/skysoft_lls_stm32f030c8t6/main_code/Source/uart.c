/*
 * uart.c
 *
 *  Created on: Jan 21, 2021
 *      Author: Thangnguyen
 */
#include <stdbool.h>

#include "uart.h"
#include "stm32f0xx_usart.h"

#define USART1_RCC					RCC_APB2Periph_USART1
#define USART2_RCC					RCC_APB1Periph_USART2

#define USART1_TX_PORT				GPIOA
#define USART1_TX_PIN				GPIO_Pin_9
#define USART1_TX_PIN_SOURCE		GPIO_PinSource9
#define USART1_TX_RCC				RCC_AHBPeriph_GPIOA

#define USART1_RX_PORT				GPIOA
#define USART1_RX_PIN				GPIO_Pin_10
#define USART1_RX_PIN_SOURCE		GPIO_PinSource10
#define USART1_RX_RCC				RCC_AHBPeriph_GPIOA

#define USART2_TX_PORT				GPIOA
#define USART2_TX_PIN				GPIO_Pin_2
#define USART2_TX_PIN_SOURCE		GPIO_PinSource2
#define USART2_TX_RCC				RCC_AHBPeriph_GPIOA

#define USART2_RX_PORT				GPIOA
#define USART2_RX_PIN				GPIO_Pin_3
#define USART2_RX_PIN_SOURCE		GPIO_PinSource3
#define USART2_RX_RCC				RCC_AHBPeriph_GPIOA

static void uart1_init(uint32_t baud);
static void uart2_init(uint32_t baud);


static void uart1_putChar(uint8_t data) ;
static uint8_t uart1_getChar(void) ;
static void uart1_putString(uint8_t *ptr);
static void uart1_putArr(uint8_t *ptr, uint16_t len);

static void uart2_putChar(uint8_t data);
static uint8_t uart2_getChar(void);
static void uart2_putString(uint8_t *ptr);
static void uart2_putArr(uint8_t *ptr, uint16_t len);

void PC_uartInit(uint32_t baud){
	uart1_init(baud);
}

void PC_uart_putChar(uint8_t data){
	uart1_putChar(data);
}

static void uart1_init(uint32_t baud) {

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(USART1_RCC,ENABLE);
	RCC_AHBPeriphClockCmd(USART1_TX_RCC,ENABLE);
	RCC_AHBPeriphClockCmd(USART1_RX_RCC,ENABLE);

	GPIO_PinAFConfig(USART1_TX_PORT, USART1_TX_PIN_SOURCE, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART1_RX_PORT, USART1_RX_PIN_SOURCE, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure the USART1 */
	USART_Init(USART1, &USART_InitStructure);
	/* Enable USART1 interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

}

static void uart2_init(uint32_t baud) {

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(USART2_RCC,ENABLE);
	RCC_AHBPeriphClockCmd(USART2_TX_RCC,ENABLE);
	RCC_AHBPeriphClockCmd(USART2_RX_RCC,ENABLE);

	GPIO_PinAFConfig(USART2_TX_PORT, USART2_TX_PIN_SOURCE, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(USART2_RX_PORT, USART2_RX_PIN_SOURCE, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure the USART2 */
	USART_Init(USART2, &USART_InitStructure);
	/* Enable USART2 interrupt */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

}


static void uart1_putChar(uint8_t data) {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
	}
	USART_SendData(USART1, data);
}

static uint8_t uart1_getChar(void) {
	uint8_t data = 0;
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		;
	data = USART_ReceiveData(USART1);
	return data;

}

static void uart1_putString(uint8_t *ptr) {
	while (*ptr) {
		uart1_putChar(*ptr);
		ptr++;
	}
}

static void uart1_putArr(uint8_t *ptr, uint16_t len) {
	while (len) {
		uart1_putChar(*ptr);
		ptr++;
		len--;
	}
}

static void uart2_putChar(uint8_t data) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {
	}
	USART_SendData(USART2, data);
}

static uint8_t uart2_getChar(void) {
	uint8_t data = 0;
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
		;
	data = USART_ReceiveData(USART2);
	return data;

}

static void uart2_putString(uint8_t *ptr) {
	while (*ptr) {
		uart2_putChar(*ptr);
		ptr++;
	}
}
static void uart2_putArr(uint8_t *ptr, uint16_t len) {
	while (len) {
		uart2_putChar(*ptr);
		ptr++;
		len--;
	}
}

