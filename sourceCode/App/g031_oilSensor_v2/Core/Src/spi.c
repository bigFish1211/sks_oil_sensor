/*
 * spi.c
 *
 *  Created on: Aug 16, 2023
 *      Author: icepe
 */


#include "spi.h"
#include "global.h"
#include "xprintf.h"
#include "drv_spi.h"

#define SPI_DUMMY_BYTE         0x00


static uint8_t SPI_SendByte(SPI_TypeDef *SPIx, uint8_t byte, uint16_t timeout);
static uint8_t SPI_readByte(SPI_TypeDef *SPIx, uint8_t*byte, uint16_t timeout);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void SPIx_init(SPI_TypeDef *SPIx){

	if (SPIx == SPI1) {
		spi1Init();
	}
	if (SPIx == SPI2) {
		spi2Init();
	}
}


int SPIx_ReadRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout){
#if 0
	reg |= 0x80;

	if(SPIx == SPI1)
		spi1_cs_low();
	else
		spi2_cs_low();

	if(!SPI_SendByte(SPIx,reg,timeout)){
		xprintf("time out \r\n");
		return 0;
	}

	while(len){

		if(!SPI_readByte(SPIx,data, timeout)){
			xprintf("time out \r\n");
			return 0;
		}
		len--;
		data++;
	}

	if(SPIx == SPI1)
		spi1_cs_high();
	else
		spi2_cs_high();

	return 1;
#else
	reg |= 0x80;
	spi2_cs_low();
	SPIx_readWrite(SPIx, reg);
	for (int i = 0; i < len; i++) {
		*(data + i) = SPIx_readWrite(SPIx, 0x00);
	}
	spi2_cs_high();
	return 1;
#endif
}

int SPIx_WriteRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout){
#if 0
	if (SPIx == SPI1)
		spi1_cs_low();
	else
		spi2_cs_low();

	if (!SPI_SendByte(SPIx, reg, timeout)) {
		xprintf("time out \r\n");
		return 0;
	}
	while (len) {

		if (!SPI_SendByte(SPIx, *data, timeout)) {
			xprintf("time out \r\n");
			return 0;
		}
		len--;
		data++;
	}

	if (SPIx == SPI1)
		spi1_cs_high();
	else
		spi2_cs_high();

	return 1;
#else
	spi2_cs_low();
	SPIx_readWrite(SPIx, reg);
	SPIx_readWrite(SPIx, *data);
	spi2_cs_high();
#endif
}


static uint8_t SPI_SendByte(SPI_TypeDef *SPIx, uint8_t byte, uint16_t timeout)
{
	uint32_t last_time =0;
	last_time = ulMiliCount+timeout;
	while (((SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE) && ((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY)) {
		if (last_time < ulMiliCount) {
		xprintf("%s time out \r\n",__func__);
			return 0;
		}
	}
	SPIx_readWrite(SPIx, byte);
	last_time = ulMiliCount+timeout;
	while (((SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE) && ((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY)) {
		if(last_time<ulMiliCount){
			xprintf("%s time out \r\n",__func__);
			return 0;
		}
	}
 //SPI_ReceiveData8(SPIx);
 SPIx_readWrite(SPIx, SPI_DUMMY_BYTE);
		return 1;
}

static uint8_t SPI_readByte(SPI_TypeDef *SPIx, uint8_t*byte, uint16_t timeout){
	uint32_t last_time = 0;
	/*!< Loop while DR register in not emplty */
	last_time = ulMiliCount + timeout;
	while (((SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE) && ((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY)) {
		if (last_time < ulMiliCount) {
			xprintf("%s time out \r\n", __func__);
			return 0;
		}
	}

	/*!< Send byte through the SPI1 peripheral */
	SPIx_readWrite(SPIx, SPI_DUMMY_BYTE);
	//SPIx->DR=SPI_DUMMY_BYTE;
	/*!< Wait to receive a byte */
	last_time = ulMiliCount + timeout;
	while (((SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE) && ((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY)) {
		if (last_time < ulMiliCount) {
			xprintf("%s time out \r\n", __func__);
			return 0;
		}
	}

	//*byte=(uint8_t)( SPIx->DR);
	*byte = SPIx_readWrite(SPIx, SPI_DUMMY_BYTE);
	return 1;
}

