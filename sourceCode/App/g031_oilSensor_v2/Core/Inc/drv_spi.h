/*
 * drv_spi.h
 *
 *  Created on: Apr 3, 2023
 *      Author: icepe
 */

#ifndef INC_DRV_SPI_H_
#define INC_DRV_SPI_H_

#include <stdint.h>
#include "stm32g0xx.h"

void spi2Init(void);
void spi2_cs_low(void);
void spi2_cs_high(void);

uint8_t SPIx_readWrite(SPI_TypeDef *SPIx, uint8_t byte);
#endif /* INC_DRV_SPI_H_ */
