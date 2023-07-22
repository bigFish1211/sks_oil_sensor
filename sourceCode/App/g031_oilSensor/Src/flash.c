/*
 * flash.c
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */
#include "flash.h"
#include "config.h"
#include "stdint.h"
#include "stm32g031xx.h"

//volatile FLASH_Status FLASHStatus = FLASH_TIMEOUT;

char NMEAChecksum(char *data, int len) {
	char checksum_value = 0;

	int string_length = len;
	int index = 1; // Skip over the $ at the begining of the sentence
	while (index < string_length) {
		checksum_value ^= data[index];
		index++;
	}
	return (checksum_value);
}

static void flash_lock(void){
	  FLASH->CR |= FLASH_CR_LOCK;
}

static void flash_unlock(void) {
	if (FLASH->CR & FLASH_CR_LOCK) {
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
	}
}

int flashEraserPage(uint32_t Page){
#if 0
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page = Page;
	EraseInitStruct.NbPages = 1;//((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
	//  Error occurred while page erase.
	  return HAL_FLASH_GetError ();
	}
	 return 1;
#else
	// flash_unlock();

	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR |= FLASH_CR_PER; //Page Erase Set
	FLASH->CR |= (FLASH_CR_PNB & (Page<<3));
	FLASH->CR |= FLASH_CR_STRT; //Start Page Erase
	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR &= ~FLASH_SR_BSY1;
	FLASH->CR &= ~FLASH_CR_PER; //Page Erase Clear

	//flash_lock();
	return 1;
#endif
}

//#define FLASH_PROGRAM_WORD

int flashWiteWord(uint32_t addr,uint32_t data){
	FLASH->CR |= FLASH_CR_PG;				/*!< Programming */
	while((FLASH->SR&FLASH_SR_BSY1));
	*(__IO uint32_t*)addr = data;
	while((FLASH->SR&FLASH_SR_BSY1));
	FLASH->CR &= ~FLASH_CR_PG;

	return 1;
}

int flash_ProgramDoubleWord(uint32_t addr, uint64_t data){

	while((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR |= FLASH_CR_PG;

	*(uint32_t*)addr = (uint32_t)data;
	__ISB();
	 *(uint32_t *)(addr + 4U) = (uint32_t)(data >> 32U);
	while((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR &= ~FLASH_CR_PG;
	return 1;
}


int FlashWritePage(uint16_t pageNum, uint8_t *pdata,uint16_t len){

	uint32_t pageAddr = (uint32_t) (pageNum * FLASH_PAGE_SIZE + FLASH_BASE);
	uint64_t *pRecord = (uint64_t*) pdata;

	flash_unlock();
	//clear flag
	FLASH->SR &= ~(FLASH_SR_EOP|FLASH_SR_WRPERR|FLASH_SR_PGAERR);
	flashEraserPage(pageNum);
	for (int i = 0; i < len; i += 8,pRecord++,pageAddr += 8) {
		if (!flash_ProgramDoubleWord(pageAddr, *pRecord)) {
			flash_lock();
			return 0;
		}
	}
	flash_lock();
	return 1;
}

int FlashReadPage(uint16_t pageNum, uint8_t *pdata,	uint16_t len){
	uint32_t pageAddr = (uint32_t)(pageNum*FLASH_PAGE_SIZE + FLASH_BASE);
	uint32_t *ptr = (uint32_t*)pdata;
	for (int i = 0; i < len; i+=4,ptr++,pageAddr+=4) {
		*ptr= *( uint32_t *)pageAddr;
	}
	return 1;
}
