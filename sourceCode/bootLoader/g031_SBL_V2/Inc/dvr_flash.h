/*
 * dvr_flash.h
 *
 *  Created on: Jan 3, 2023
 *      Author: icepe
 */

#ifndef DVR_FLASH_H_
#define DVR_FLASH_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32g0xx.h"

#define BASE_ADDR                        (0x8000000)


#define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
#define BASE_ADDR                        (0x8000000)    /* 2 Kbytes */
//#define FLASH_SIZE                        (0x80000)  /* 512 KBytes */

#define FLASH_FLAG_BSY                 ((uint32_t)0x00010000)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000001)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PROGERR             ((uint32_t)0x00000008)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag */

#define FLASH_FLAG_BANK1_BSY           FLASH_FLAG_BSY       /*!< FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP           FLASH_FLAG_EOP       /*!< FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_PROGERR       FLASH_FLAG_PROGERR     /*!< FLASH BANK1 Program error flag */
#define FLASH_FLAG_BANK1_WRPRTERR      FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 Write protected error flag */

/* FLASH Keys */
#define FLASH_KEY1                     0x45670123U
#define FLASH_KEY2                     0xCDEF89ABU

#define FLASH_OPTKEY1                   0x08192A3BU   /*!< Flash option byte key1 */
#define FLASH_OPTKEY2                   0x4C5D6E7FU   /*!< Flash option byte key2: used with FLASH_OPTKEY1

/* FLASH BANK address */
#define FLASH_BANK1_END_ADDRESS   		((uint32_t)0x803FFFF)

/* Delay definition */
#define EraseTimeout          			((uint32_t)0x000B0000)
#define ProgramTimeout        			((uint32_t)0x00002000)

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess);
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer);
void FLASH_Unlock(void);
void FLASH_Lock(void);
void FLASH_OB_Unlock(void);
void FLASH_OB_Lock(void);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastBankOperation_(uint32_t Timeout);
FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout);
FLASH_Status FLASH_GetBank2Status(void);
FLASH_Status FLASH_WaitForLastBank2Operation(uint32_t Timeout);
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_EraseAllPages(void);
FLASH_Status FLASH_EraseOptionBytes(void);
void flashProgramDoubleWorld(uint32_t addr,uint64_t data);
FLASH_Status FLASH_ProgramDoubleWord(uint32_t addr,uint64_t data);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data);
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages);
void FLASH_ReadOutProtection(uint32_t RDPLevel);
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY);
uint32_t FLASH_GetUserOptionByte(void);
uint32_t FLASH_GetWriteProtectionOptionByte(void);
uint32_t FLASH_GetReadOutProtectionStatus(void);
//FlagStatus FLASH_GetPrefetchBufferStatus(void);
//void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
//FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);


int FlashWritePage(uint32_t addr, uint8_t *pdata,uint16_t len);
#endif /* DVR_FLASH_H_ */
