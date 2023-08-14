/*
 * flash.h
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */
#include <stdio.h>

//#define FLASH_PAGE_SIZE    				((uint16_t)0x400)
#ifdef STM32G031xx

#define FLASH_TOTAL_PAGE
#define FLASH_START_ADDR				((uint32_t)0x08000000)

#define FLASH_START_CONFIG_PAGE			31
#define FLASH_START_CONFIG_ADDR  		(uint32_t)(FLASH_START_ADDR+FLASH_BASE*FLASH_PAGE_SIZE)

#define NUM_CONFIG_PAGE 				1
#define CONFIG_END_ADDR 				(uint32_t)(FLASH_START_ADDR+FLASH_PAGE_SIZE*NUM_CONFIG_PAGE)


#define FLASH_APPLICATION_PAGE			5UL
#define FLASH_APPLICATION_ADDR			(uint32_t)(FLASH_START_ADDR+FLASH_PAGE_SIZE*FLASH_APPLICATION_PAGE)

#define FLASH_FLAG_BSY                 ((uint32_t)0x00010000)  /*!< FLASH Busy flag */
//#define FLASH_FLAG_EOP                 ((uint32_t)0x00000001)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PGERR               ((uint32_t)0x00000008)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag */
//#define FLASH_FLAG_OPTERR              ((uint32_t)0x00000000)  /*!< FLASH Option Byte error flag */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 Program error flag */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 Write protected error flag */

#endif

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_EraseAllPages(void);
FLASH_Status FLASH_EraseOptionBytes(void);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data);
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages);
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState);
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY);

