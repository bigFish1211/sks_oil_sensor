/*
 * drv_spi.c
 *
 *  Created on: Apr 3, 2023
 *      Author: icepe
 */
#include "drv_spi.h"

extern SPI_HandleTypeDef hspi2;

#define _W25QXX_SPI 		hspi2

#define SPI2_MISO_PIN 		14
#define SPI2_MISO_PORT 		GPIOB

#define SPI2_MOSI_PIN 		15
#define SPI2_MOSI_PORT 		GPIOB

#define SPI2_SCK_PIN 		13
#define SPI2_SCK_PORT 		GPIOB

#define SPI2_CS_PIN 		12
#define SPI2_CS_PORT 		GPIOB

void SPI2_LowLevel_Init(void) {
	uint32_t temp;
	//MISO
	SPI2_MISO_PORT->MODER &= ~(0x03 << (SPI2_MISO_PIN * 2));
	SPI2_MISO_PORT->MODER |= (0x02 << (SPI2_MISO_PIN * 2)); //mode AF

	SPI2_MISO_PORT->ODR &= ~(0x01 << SPI2_MISO_PIN); //push pull
	SPI2_MISO_PORT->PUPDR &= ~(0x03 << (SPI2_MISO_PIN * 2)); //no pull

	//SPI2_MISO_PORT->AFR[1] = 0x00;

	temp = SPI2_MISO_PORT->AFR[SPI2_MISO_PIN >> 3u];
	temp &= ~(0xFu << ((SPI2_MISO_PIN & 0x07u) * 4u));
	temp |= ((0x00) << ((SPI2_MISO_PIN & 0x07u) * 4u));
	SPI2_MISO_PORT->AFR[SPI2_MISO_PIN >> 3u] = temp;

	//MOSI
	SPI2_MOSI_PORT->MODER &= ~(0x03 << (SPI2_MOSI_PIN * 2));
	SPI2_MOSI_PORT->MODER |= (0x02 << (SPI2_MOSI_PIN * 2)); //mode AF

	SPI2_MOSI_PORT->ODR &= ~(0x01 << SPI2_MOSI_PIN); //push pull
	SPI2_MOSI_PORT->PUPDR &= ~(0x03 << (SPI2_MOSI_PIN * 2)); //no pull

	//SPI2_MOSI_PORT->AFR[1] = 0x00;
	temp = SPI2_MOSI_PORT->AFR[SPI2_MOSI_PIN >> 3u];
	temp &= ~(0xFu << ((SPI2_MOSI_PIN & 0x07u) * 4u));
	temp |= ((0x00) << ((SPI2_MOSI_PIN & 0x07u) * 4u));
	SPI2_MOSI_PORT->AFR[SPI2_MOSI_PIN >> 3u] = temp;

	//SCK
	SPI2_SCK_PORT->MODER &= ~(0x03 << (SPI2_SCK_PIN * 2));
	SPI2_SCK_PORT->MODER |= (0x02 << (SPI2_SCK_PIN * 2)); //mode AF

	SPI2_SCK_PORT->ODR &= ~(0x01 << SPI2_SCK_PIN); //push pull
	SPI2_SCK_PORT->PUPDR &= ~(0x03 << (SPI2_SCK_PIN * 2)); //no pull

	//SPI2_SCK_PORT->AFR[1] = 0x00;
	temp = SPI2_SCK_PORT->AFR[SPI2_SCK_PIN >> 3u];
	temp &= ~(0xFu << ((SPI2_SCK_PIN & 0x07u) * 4u));
	temp |= ((0x00) << ((SPI2_SCK_PIN & 0x07u) * 4u));
	SPI2_SCK_PORT->AFR[SPI2_SCK_PIN >> 3u] = temp;

}


void SPIx_Init(SPI_TypeDef *SPIx){
	 RCC->APBENR1 |= RCC_APBENR1_SPI2EN;//clock Enbale

	 SPIx->CR1 |= SPI_CR1_MSTR; //MasterMode
	 SPIx->CR1 &= ~SPI_CR1_RXONLY;// 2line
	 SPIx->CR2 &= SPI_CR2_DS;
	 SPIx->CR2 &= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;//8bit
	 SPIx->CR1 &= ~SPI_CR1_CPHA;
	 SPIx->CR1 &= ~SPI_CR1_CPOL;
	 SPIx->CR1 |= SPI_CR1_SSM;//soff nss
	 SPIx->CR1 |= SPI_CR1_SSI;//soff nss
	 SPIx->CR1 &= ~SPI_CR1_BR;
	 SPIx->CR1 |= (SPI_CR1_BR_1 | SPI_CR1_BR_2);   //f/8
	 SPIx->CR1 &= ~SPI_CR1_LSBFIRST; //SPI_FIRSTBIT_MSB
	 SPIx->CR2 &= ~SPI_CR2_FRF;//SPI_TIMODE_DISABLE
	 SPIx->CR1 &= ~SPI_CR1_CRCEN;//SPI_CRCCALCULATION_DISABLE
	 SPIx->CR2 |= SPI_CR2_NSSP;//SPI_NSS_PULSE_ENABLE
	 SPIx->CR2 |= SPI_CR2_FRXTH;
	 SPIx->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;//SPI mode is selected
	 SPIx->CR1 |= SPI_CR1_SPE;//enable SPI

}


int valu=0;

uint8_t SPIx_readWrite(SPI_TypeDef *SPIx, uint8_t byte){
	uint8_t ret  = 0;

	valu = byte;
	while(!((SPIx->SR) & SPI_SR_TXE)){}
	*((__IO uint8_t *)&SPIx->DR) = byte;
	while(!((SPIx->SR) & SPI_SR_RXNE));
	ret =  (uint8_t)SPIx->DR;
	return ret;
}


