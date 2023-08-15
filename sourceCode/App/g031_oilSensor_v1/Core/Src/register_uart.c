/*
 * register_uart.c
 *
 *  Created on: May 4, 2023
 *      Author: KhanhNguyen
 */

#include "register_uart.h"

static void uart1_Int(uint32_t baudrate);
static void uart1_PutChar(uint8_t ch);
static void uart1_PutString(uint8_t *str);
static void uart1_putArray(uint8_t *str, int len);


void pc_Int(uint32_t bau){
	uart1_Int(bau);
}

void pc_putArray(uint8_t *str, int len){
	uart1_putArray(str, len);
}

static void uart1_Int(uint32_t baudrate){
	uint32_t BRR_value = SYS_CLOCK/baudrate;
	uint32_t* NVIC_ISER0 = (uint32_t)0xe000e100;

	// clock enable PA, UART
	uint32_t *RCC_IOPENR  = (uint32_t*)0x40021034;
	uint32_t *RCC_APBENR2 = (uint32_t*)0x40021040;

	*RCC_IOPENR  |= 1<<0;
	*RCC_APBENR2 |= 1<<14;

	//////////////////////////////////////
	// uart1 configure alternate
    // PA10     ------> USART1_RX
    // PA9      ------> USART1_TX

	uint8_t PINx = 2;
	uint8_t AF_PINx = 4;

	// PA9 TX
	REGISTER_GPIOA->MODER &= ~(0b11<<(9*PINx));
	REGISTER_GPIOA->MODER |= 0b10<<(9*PINx);			// alternate moder

	REGISTER_GPIOA->PUPDR   &= ~(0b11<<(9*PINx));		// no full
	REGISTER_GPIOA->OSPEEDR &= ~(0b11<<(9*PINx));		// Very low speed
	REGISTER_GPIOA->AFRH    &= ~(0b1111<<(1*AF_PINx));	// AF1
	REGISTER_GPIOA->AFRH    |= 0b0001<<(1*AF_PINx);	// AF1

	// PA10 RX
	REGISTER_GPIOA->MODER &= ~(0b11<<(10*PINx));
	REGISTER_GPIOA->MODER |=   0b10<<(10*PINx);			// alternate moder

	REGISTER_GPIOA->PUPDR   &= ~(0b11<<(10*PINx));		// no full
	REGISTER_GPIOA->OSPEEDR &= ~(0b11<<(10*PINx));		// Very low speed
	REGISTER_GPIOA->AFRH    &= ~(0b1111<<(2*AF_PINx));	// AF1
	REGISTER_GPIOA->AFRH    |= 0b0001<<(2*AF_PINx);	// AF1

///////////////////////////////////////////////////////////////////////////////

	REGISTER_UART1->CR1 &= ~1<<0;		// UE: disable
	REGISTER_UART1->CR1 &= ~1<<10;		// PCE: parity disable
	REGISTER_UART1->CR1 &= ~1<<15;		// OVER8: OverSampling 16bit
	REGISTER_UART1->CR1 &= ~1<<28;		// M1: Word length 8n1

	REGISTER_UART1->CR1 |= 1<<3;		// TE: enable
	REGISTER_UART1->CR1 |= 1<<2;		// RE: enable
	REGISTER_UART1->CR1 |= 1<<5;		// RXNEIE: RX interrupt

	REGISTER_UART1->PRESC &= ~(0b1111<<0);	//ClockPrescaler no div

	//set baud rate 115200
	REGISTER_UART1->BRR = BRR_value+1;		// BRR = 0x22C;
	*NVIC_ISER0 |= 1<<27;					// uart1 interrupt
	REGISTER_UART1->CR1 |= 1<<0;			// UE: enable

}

static void uart1_PutChar(uint8_t ch) {
	uint32_t* uart1_ISR = (uint32_t*)0x4001381c;
	uint32_t* uart1_TDR = (uint32_t*)0x40013828;

	while (((REGISTER_UART1->ISR >> 7) & 1) != 1) {}
	REGISTER_UART1->TDR = ch;
}

static void uart1_PutString(uint8_t* str) {
	while (*str) {
		register_uart1_PutChar(*str);
		str++;
	}
}

static void uart1_putArray(uint8_t* str, int len) {
	while (len) {
		uart1_PutChar(*str);
		str++;
		len--;
	}
}
