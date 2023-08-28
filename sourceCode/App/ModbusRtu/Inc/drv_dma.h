/*
 * drv_dma.h
 *
 *  Created on: Aug 18, 2023
 *      Author: icepe
 */

#ifndef DRV_DMA_H_
#define DRV_DMA_H_

#include "stdint.h"
#include "stm32g031xx.h"

void dma_init(DMA_Channel_TypeDef *DMA_channelx);
void dma_config(DMA_Channel_TypeDef *DMA_channelx,uint32_t srcAdd, uint32_t destAdd, uint16_t datasize);
#endif /* DRV_DMA_H_ */
