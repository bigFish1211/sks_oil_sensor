/*
 * flash.h
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */
#include <stdio.h>

//#define FLASH_PAGE_SIZE    				((uint16_t)0x400)
#ifdef STM32G031xx

#define FLASH_KEY1                      0x45670123U
#define FLASH_KEY2                      0xCDEF89ABU

#define FLASH_PAGE_SIZE                 0x00000800U    /*!< FLASH Page Size, 2 KBytes */

#define FLASH_TOTAL_PAGE
#define FLASH_START_ADDR				((uint32_t)0x08000000)

#define FLASH_START_CONFIG_PAGE			31
#define FLASH_START_CONFIG_ADDR  		(uint32_t)(FLASH_START_ADDR+FLASH_BASE*FLASH_PAGE_SIZE)

#define NUM_CONFIG_PAGE 				1
#define CONFIG_END_ADDR 				(uint32_t)(FLASH_START_ADDR+FLASH_PAGE_SIZE*NUM_CONFIG_PAGE)


#define FLASH_APPLICATION_PAGE			5UL
#define FLASH_APPLICATION_ADDR			(uint32_t)(FLASH_START_ADDR+FLASH_PAGE_SIZE*FLASH_APPLICATION_PAGE)

#endif

int fashEraserPage(uint32_t PageAddress);
int FlashWritePage(uint16_t pageNum, uint8_t *pdata,	uint16_t len);
int FlashReadPage(uint16_t pageNum, uint8_t *pdata,	uint16_t len);
