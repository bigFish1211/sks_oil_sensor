/*
 * uart.c
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */

#include <dvr_uart.h>
#include "global.h"

UART_HandleTypeDef huart1;

static void MX_USART1_UART_Init(uint32_t baud);


static void uart1_putChar(uint8_t ch);
static uint8_t uart1_getChar(uint8_t *ch);
static void uart1_putString(uint8_t *str);
static void uart1_putArray(uint8_t *str, int len);

void comm_init(void){

	MX_USART1_UART_Init(PC_COMM_BAUD);
}
void SerialPutChar(uint8_t c) {
	uart1_putChar(c);
}

uint32_t SerialKeyPressed(uint8_t *key) {
	if(USART_ISR_TXE_TXFNF == (USART1->ISR & USART_ISR_TXE_TXFNF)){
		*key = (uint8_t) USART1->RDR;
		return 1;
	}else{
		return 0;
	}
}

int32_t Rcv_Byte(uint8_t *c, uint32_t timeout) {
	uint32_t sec = g_appStats.secCount;
	while (1) {
		if (SerialKeyPressed(c) == 1) {
			return 0;
		}

		if ((uint32_t)(g_appStats.secCount - sec) >= timeout)
			return -1;
	}
	return -1;
}

int32_t RcvBuff(uint8_t *buff, int *length, uint32_t timeout) {
	int i = 0;
	int len = *length;
	*length = 0;
	for (i = 0; i < len; i++) {
		if (Rcv_Byte(buff + i, timeout) != 0) {
			break;
		}
		(*length)++;
	}
	return *length;
}

static void uart1_putChar(uint8_t ch){
	while ((USART1->ISR & USART_ISR_TXE_TXFNF) != USART_ISR_TXE_TXFNF);
	USART1->TDR = ch;

}

static uint8_t uart1_getChar(uint8_t *ch){
	while ((USART1->ISR & USART_ISR_TXE_TXFNF) != USART_ISR_TXE_TXFNF);
	USART1->TDR = ch;

}

static void uart1_putString(uint8_t *str){
	while(*str){
		uart1_putChar(*str);
		str++;
	}

}
static void uart1_putArray(uint8_t *str, int len){
	while(len){
		uart1_putChar(*str);
		str++;
		len--;
	}

}


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(uint32_t baud)
{
  /* Peripheral clock enable */
  	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  	RCC->APBENR2 |= RCC_APBENR2_USART1EN;

  	/**USART1 GPIO Configuration
  	    PA10     ------> USART1_RX
  	    PA9    ------> USART1_TX
  	    */
  	/*PB6     ------> USART1_TX*/
  	//mode AF
  	GPIOA->MODER &= ~GPIO_MODER_MODE9;
  	GPIOA->MODER |=  GPIO_MODER_MODE9_1;//mode AF
  	//mode push-pull
  	GPIOA ->OTYPER &= ~GPIO_OTYPER_OT9;
  	//type noPull
  	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;
  	//speed low
  	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9;

  	/*PB7     ------> USART1_RX*/
  	//modeAF
  	GPIOA->MODER &= ~GPIO_MODER_MODE10;
  	GPIOA->MODER |=  GPIO_MODER_MODE10_1;//mode AF
	//mode push-pull
  	GPIOA ->OTYPER &= ~GPIO_OTYPER_OT10;
  	//type noPull
  	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10;
  	//speed low
  	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED10;
  	//sel AF0 for uart
  	/*GPIOA->AFR[0] &= ~GPIO_AFRH_AFSEL9;//AF1
  	GPIOA->AFR[0] &= ~GPIO_AFRH_AFSEL10;//AF1S*/
  	//0b0001

  	GPIOA->AFR[1] |=  GPIO_AFRH_AFSEL9_0;//AF1
  	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9_1;
  	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9_2;
  	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9_3;
  	//0b001
	GPIOA->AFR[1] |=  GPIO_AFRH_AFSEL10_0;//AF1
  	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10_1;
  	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10_2;
  	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10_3;


  	USART1->CR1 = 0;
  	// Word length 8n1
  	USART1->CR1 &= ~USART_CR1_M1;
  	//Parity disable
  	USART1->CR1 &= ~USART_CR1_PCE;
  	//mode Tx_Rx
  	USART1->CR1 |= USART_CR1_TE;
  	USART1->CR1 |= USART_CR1_RE;
  	//OverSampling 16bit
  	USART1->CR1 &= ~USART_CR1_OVER8;
  	//ClockPrescaler no div
  	USART1->PRESC &= USART_PRESC_PRESCALER;
  	//set baudrate 115200
	USART1->BRR = 0xd05;
  	//USART1->BRR = 0x22b;

  	//rx interupt enable
  	USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE;

  	//set uart1 interrupt
  	NVIC_SetPriority(USART1_IRQn, 0);
  	NVIC_EnableIRQ(USART1_IRQn);

  	//uart enable
  	USART1->CR1 |= USART_CR1_UE;
}

