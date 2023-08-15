/*
 * flash.c
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */
#include "flash.h"
#include "config.h"
#include "define.h"
#include "stm32g031xx.h"

#include <stdint.h>
#include <string.h>
//volatile FLASH_Status FLASHStatus = FLASH_TIMEOUT;

char NMEAChecksum(uint8_t *data, int len) {
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

	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR |= FLASH_CR_PER; //Page Erase Set
	FLASH->CR |= (FLASH_CR_PNB & (Page<<3));
	FLASH->CR |= FLASH_CR_STRT; //Start Page Erase
	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR &= ~FLASH_SR_BSY1;
	FLASH->CR &= ~FLASH_CR_PER; //Page Erase Clear

	return 1;
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

void write_config(void) {
	printf("SKS_OIL_SENSOR write config\r\n");
	uint8_t buffConfig[SKS_CONFIG_RECORD_SIZE] = { 0 };
	unsigned int pointerSize = sizeof(SENSOR_CONFIG_T);
	unsigned int size =
			pointerSize < SKS_CONFIG_RECORD_SIZE ?
					pointerSize : SKS_CONFIG_RECORD_SIZE;
	memcpy(buffConfig, &sensorConfig, size);
	buffConfig[SKS_CONFIG_RECORD_SIZE - 1] = NMEAChecksum(buffConfig,
			SKS_CONFIG_RECORD_SIZE - 1);
	FlashWritePage(FLASH_START_CONFIG_PAGE, buffConfig, SKS_CONFIG_RECORD_SIZE);
}

static void make_default_config(void) {
	printf("SKS_OIL_SENSOR write default config\r\n");
	sensorConfig.sign = CONFIG_SIGN;
	sensorConfig.ema = EMA5;
	sensorConfig.update_freq = 1000;//ms
	sensorConfig.full = 0;
	sensorConfig.empty = 0;
	write_config();
}

void load_config(void){
	printf("SKS_OIL_SENSOR load_config\r\n");
	uint8_t buffConfig[SKS_CONFIG_RECORD_SIZE] = { 0 };

	FlashReadPage(FLASH_START_CONFIG_PAGE, buffConfig,
	SKS_CONFIG_RECORD_SIZE);

	char crc = NMEAChecksum(buffConfig, SKS_CONFIG_RECORD_SIZE - 1);
	if (buffConfig[0] == CONFIG_SIGN
			&& crc == buffConfig[SKS_CONFIG_RECORD_SIZE - 1]) {
		unsigned int pointerSize = sizeof(SENSOR_CONFIG_T);
		unsigned int size =
				pointerSize < SKS_CONFIG_RECORD_SIZE ?
						pointerSize : SKS_CONFIG_RECORD_SIZE;
		memcpy(&sensorConfig, buffConfig, size);
	} else {
		make_default_config();
	}
}
