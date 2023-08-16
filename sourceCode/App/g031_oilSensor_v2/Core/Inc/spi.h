/*
 * spi.h
 *
 *  Created on: Aug 16, 2023
 *      Author: icepe
 */

#include  <stdint.h>
#include  "stm32g031xx.h"

void SPIx_init(SPI_TypeDef *SPIx);
int SPIx_ReadRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout);
int SPIx_WriteRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout);
int SPIx_ReadRegister_t(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout);
