/*
 * crc.h
 *
 *  Created on: Oct 25, 2022
 *      Author: icepe
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_
#include "stdint.h"
#include "stm32g031xx.h"

void CRC_ResetDR(void);
uint32_t CRC_CalcCRC(uint32_t Data);
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);
uint32_t CRC_GetCRC(void);
void CRC_SetIDRegister(uint8_t IDValue);
uint8_t CRC_GetIDRegister(void);


#endif /* INC_CRC_H_ */
