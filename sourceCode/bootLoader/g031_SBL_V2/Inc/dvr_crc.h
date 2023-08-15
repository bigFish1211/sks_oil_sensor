/*
 * dvr_crc.h
 *
 *  Created on: Jan 3, 2023
 *      Author: thangnq
 */

#ifndef DVR_CRC_H_
#define DVR_CRC_H_

#include <stdint.h>

void CRC_ClockInit(void);
void CRC_ResetDR(void);
uint32_t CRC_CalcCRC(uint32_t Data);
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);
uint32_t CRC_GetCRC(void);
void CRC_SetIDRegister(uint8_t IDValue);
uint8_t CRC_GetIDRegister(void);

#endif /* DVR_CRC_H_ */
