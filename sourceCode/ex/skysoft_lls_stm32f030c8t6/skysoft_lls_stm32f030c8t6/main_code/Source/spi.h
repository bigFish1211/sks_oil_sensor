#ifndef SPI_H_
#define SPI_H_
#include <stdint.h>
#include "stm32f0xx_conf.h"


void SPIx_init(SPI_TypeDef *SPIx);
int SPIx_ReadRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout);
int SPIx_WriteRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout);
int SPIx_ReadRegister_t(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout);
#endif
