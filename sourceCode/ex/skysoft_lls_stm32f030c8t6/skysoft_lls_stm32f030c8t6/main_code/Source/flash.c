/*
 * EEprom.c
 *
 *  Created on: Feb 16, 2017
 *      Author: ThangNguyen
 */
#include <string.h>
#include "xprintf.h"
#include "stm32f0xx_flash.h"

#include "DataUtil.h"
#include "config.h"
#include "flash.h"

volatile FLASH_Status FLASHStatus = FLASH_TIMEOUT;

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

void FlashInit(void) {
	FLASH_Unlock();
	/*	PageCount = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR)
	 / FLASH_PAGE_SIZE;*/
}

void flash_wrire_char(char c) {
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	FLASH_ErasePage(BANK1_WRITE_START_ADDR);
	{
		FLASH_ProgramHalfWord(BANK1_WRITE_START_ADDR, c);
	}
}

#if 0
char flash_read_char(uint32_t address) {
	return (*(char*) address);
}

uint32_t Readflash(uint32_t addr)
{
	uint32_t* data = (uint32_t*)(addr);
	return *data;
}
#endif

void flash_write_config(uint32_t address, char *pdata, int len) {

	int i;
	uint32_t *pRecord = (uint32_t*)pdata;
	uint32_t flash_address = address;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	if(FLASH_ErasePage(flash_address) != FLASH_COMPLETE)
		xprintf("erase flash fail\r\n");
	for (i = 0; i < len; i+=4,pRecord++,flash_address+=4) {
		if(FLASH_ProgramWord(flash_address, *pRecord)!=FLASH_COMPLETE)
			xprintf("write flash fail\r\n");
	}
	FLASH_Lock();
}

void flash_read_config(uint32_t address, char *pdata, int len) {
	int i;
	uint32_t flash_address = address;
	uint32_t *ptr = (uint32_t*)pdata;
	for (i = 0; i < len; i+=4,ptr++,flash_address+=4) {
		*ptr=*(__IO uint32_t *)flash_address;
	}
}

void write_config(void) {
	xprintf("write_config\r\n");
	g_config.sign = SF_CONFIG_SIGN;
	g_config.configState = 2;
	char buffConfig[SKS_CONFIG_RECORD_SIZE] = { 0 };
	int pointerSize = sizeof(DEVICE_CONFIG);
	int size =
			pointerSize < SKS_CONFIG_RECORD_SIZE ?
					pointerSize : SKS_CONFIG_RECORD_SIZE;
	memcpy(buffConfig, &g_config, size);
	buffConfig[SKS_CONFIG_RECORD_SIZE - 1] = NMEAChecksum(buffConfig, SKS_CONFIG_RECORD_SIZE - 1);
	flash_write_config(BANK1_CONFIG_START_ADDR, buffConfig,	SKS_CONFIG_RECORD_SIZE);
}

void make_default_config() {
	if(g_config.configState != 2) {
		//xprintf(, "1234567890");

		sprintf(g_config.deviceCode, "%s", "1234567890");

		g_config.emptyValue = 0;
		g_config.fullValue = 0;
		g_config.timeout3D = 3; //3 minutes
		g_config.outputMode = 0;
		g_config.diffX = 6;
		g_config.diffY = 6;
		g_config.interval = 1;

		g_config.configState = 2;

		g_config.sign = SF_CONFIG_SIGN;

		write_config();

		xprintf("\r\nWrite default config.\r\n\r\n");
	} else {
		//xprintf("\r\nLoaded config.\r\n\r\n");
	}
}

void load_config(void) {
	xprintf("load_config\r\n");
	char buffConfig[SKS_CONFIG_RECORD_SIZE] = { 0 };

	flash_read_config(BANK1_CONFIG_START_ADDR, buffConfig,
	SKS_CONFIG_RECORD_SIZE);
	char crc = NMEAChecksum(buffConfig, SKS_CONFIG_RECORD_SIZE - 1);
	if ((buffConfig[0] == SF_CONFIG_SIGN) && (crc == buffConfig[SKS_CONFIG_RECORD_SIZE - 1]))
	{
		int pointerSize = sizeof(DEVICE_CONFIG);
		int size =
				pointerSize < SKS_CONFIG_RECORD_SIZE ?
						pointerSize : SKS_CONFIG_RECORD_SIZE;
		memcpy(&g_config, buffConfig, size);
	}

	//check and make default config
	make_default_config();

	if(g_config.diffX < 3) {
		g_config.diffX = 5;

		if(g_config.timeout3D <= 1) {
			g_config.timeout3D = 3;
		}
	}

	if(g_config.diffY < 3) {
		g_config.diffY = 5;

		if(g_config.timeout3D <= 1) {
			g_config.timeout3D = 3;
		}
	}

	if(g_config.diffX > 30) {
		g_config.diffX = 5;
	}

	if(g_config.diffY > 30) {
		g_config.diffY = 5;
	}
}
