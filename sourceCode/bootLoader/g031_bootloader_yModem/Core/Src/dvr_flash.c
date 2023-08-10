/*
 * flash.c
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */
#include <dvr_flash.h>
#include "main.h"
#include "config.h"
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


FLASH_Status FLASH_ErasePage(uint32_t Page_Address){

	FLASH_Status status = FLASH_COMPLETE;
	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR |= FLASH_CR_PER; //Page Erase Set
	FLASH->CR |= (FLASH_CR_PNB & (Page_Address<<3));
	FLASH->CR |= FLASH_CR_STRT; //Start Page Erase
	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR &= ~FLASH_SR_BSY1;
	FLASH->CR &= ~FLASH_CR_PER; //Page Erase Clear
	return status;
}


FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data){

	FLASH_Status status = FLASH_COMPLETE;
	FLASH->CR |= FLASH_CR_PG;				/*!< Programming */
	while((FLASH->SR&FLASH_SR_BSY1));
	*(__IO uint32_t*)Address = Data;
	while((FLASH->SR&FLASH_SR_BSY1));
	FLASH->CR &= ~FLASH_CR_PG;

	return status;
}

FLASH_Status flash_ProgramDoubleWord(uint32_t Address, uint64_t data){

	while((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR |= FLASH_CR_PG;

	*(uint32_t*)addr = (uint32_t)Data;
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
	//clear flag;
	__HAL_FLASH_CLEAR_FLAG(
				FLASH_FLAG_EOP |FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR);
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
