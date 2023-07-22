/*
 * uart.c
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */

#include "uart.h"
#include "global.h"

#include "stm32g031xx.h"

#define SYS_CLOCK 				64000000ul

#define COMX0                      			USART1
#define COMX0_APB_CLK              			APBENR2
#define COMX0_CLK                  			RCC_APBENR2_USART1EN
#define COMX0_TX_PIN               			9		//Pin A9
#define COMX0_TX_PIN_AF            			PIN_AF_1
#define COMX0_TX_GPIO_PORT         			GPIOA
#define COMX0_TX_GPIO_CLK          			RCC_IOPENR_GPIOAEN
#define COMX0_RX_PIN               			10
#define COMX0_RX_PIN_AF            			PIN_AF_1
#define COMX0_RX_GPIO_PORT         			GPIOA				//Pin A10
#define COMX0_RX_GPIO_CLK          			RCC_IOPENR_GPIOAEN
#define COMX0_IRQn                 			USART1_IRQn
#define COMX0_IRQHandler		    		USART1_IRQn_IRQHandler

static void COMX0_pin_init(void) {
	uint32_t temp = 0;
	RCC->IOPENR |= (COMX0_TX_GPIO_CLK | COMX0_TX_GPIO_CLK);
	RCC->COMX0_APB_CLK |= COMX0_CLK;

	COMX0_RX_GPIO_PORT->MODER &= ~(0x03 << (COMX0_RX_PIN * 2));
	COMX0_RX_GPIO_PORT->MODER |= (0x02 << (COMX0_RX_PIN * 2)); //mode AF
	COMX0_RX_GPIO_PORT->ODR &= ~(0x01 << COMX0_RX_PIN); //push pull
	COMX0_RX_GPIO_PORT->PUPDR &= ~(0x03 << (COMX0_RX_PIN * 2)); //no pull
	COMX0_RX_GPIO_PORT->OSPEEDR &= ~(0x01 << COMX0_RX_PIN); ////speed low

	COMX0_TX_GPIO_PORT->MODER &= ~(0x03 << (COMX0_TX_PIN * 2));
	COMX0_TX_GPIO_PORT->MODER |= (0x02 << (COMX0_TX_PIN * 2)); //mode AF
	COMX0_TX_GPIO_PORT->ODR &= ~(0x01 << COMX0_TX_PIN); //push pull
	COMX0_TX_GPIO_PORT->PUPDR &= ~(0x03 << (COMX0_TX_PIN * 2)); //no pull
	COMX0_TX_GPIO_PORT->OSPEEDR &= ~(0x01 << COMX0_TX_PIN); ////speed low

	temp = COMX0_RX_GPIO_PORT->AFR[COMX0_RX_PIN >> 3u];
	temp &= ~(0xful << ((COMX0_RX_PIN & 0x07u) * 4ul));
	temp |= (COMX0_RX_PIN_AF << ((COMX0_RX_PIN & 0x07u) * 4ul));
	COMX0_RX_GPIO_PORT->AFR[COMX0_RX_PIN >> 3ul] = temp;

	temp = COMX0_TX_GPIO_PORT->AFR[COMX0_TX_PIN >> 3u];
	temp &= ~(0xful << ((COMX0_TX_PIN & 0x07u) * 4ul));
	temp |= (COMX0_TX_PIN_AF << ((COMX0_TX_PIN & 0x07u) * 4ul));
	COMX0_TX_GPIO_PORT->AFR[COMX0_TX_PIN >> 3ul] = temp;
}

static void uartx_init(USART_TypeDef *UARTx, uint32_t baudrate) {
	uint32_t BRR_value = SYS_CLOCK / baudrate;

	UARTx->CR1 = 0;
	// Word length 8n1
	UARTx->CR1 &= ~USART_CR1_M1;
	//Parity disable
	UARTx->CR1 &= ~USART_CR1_PCE;
	//mode Tx_Rx
	UARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	//OverSampling 16bit
	UARTx->CR1 &= ~USART_CR1_OVER8;
	//ClockPrescaler no div
	UARTx->PRESC &= USART_PRESC_PRESCALER;
	//set baudrate 115200
	UARTx->BRR = BRR_value;
	//rx interupt enable
	UARTx->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
	UARTx->CR1 |= USART_CR1_UE;
}

void usart_x0_init(uint32_t baud) {
	COMX0_pin_init();
	uartx_init(COMX0,baud);
	NVIC_SetPriority(COMX0_IRQn, 0);
	NVIC_EnableIRQ(COMX0_IRQn);
}

void usart_x0_sendChar(uint8_t c) {
	while ((COMX0->ISR & USART_ISR_TXE_TXFNF) != USART_ISR_TXE_TXFNF);
	COMX0->TDR = c;
}

void USART1_IRQHandler(void) {

	uint8_t cChar;
	if (USART1->ISR & USART_ISR_RXNE_RXFNE) {
		cChar = USART1->RDR;
		USART1->ICR |= USART_RQR_RXFRQ;
	}
}

