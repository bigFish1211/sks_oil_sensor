/*
 * EEprom.c
 *
 *  Created on: Feb 16, 2017
 *      Author: ThangNguyen
 */
#include <string.h>
#include "xprintf.h"
#include "dvr_flash.h"

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

int flash_write_config(uint32_t address, char *pdata, int len) {
	uint32_t *pRecord = (uint64_t*) pdata;
	uint32_t flash_address = address;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPRTERR);
	if (FLASH_ErasePage(flash_address) != FLASH_COMPLETE1)
		xprintf("erase flash fail\r\n");

	for (int i = 0; i < len; i += 8, pRecord++, flash_address += 8) {
		if (!FLASH_ProgramDoubleWord(flash_address, *pRecord)) {
			FLASH_Lock();
			return 0;
		}
		if (*(uint64_t*) flash_address != *pRecord) {
			return 0;
		}
	}
	FLASH_Lock();
	return 1;
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
	buffConfig[SKS_CONFIG_RECORD_SIZE - 1] = NMEAChecksum(buffConfig, SKS_CONFIG_RECORD_SIZE - 2);
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
	}
}

void load_config(void) {
	xprintf("load_config\r\n");
	char buffConfig[SKS_CONFIG_RECORD_SIZE] = { 0 };

	flash_read_config(BANK1_CONFIG_START_ADDR, buffConfig,
	SKS_CONFIG_RECORD_SIZE);
	char crc = NMEAChecksum(buffConfig, SKS_CONFIG_RECORD_SIZE - 2);
	if ((buffConfig[0] == SF_CONFIG_SIGN) && (crc == buffConfig[SKS_CONFIG_RECORD_SIZE - 2]))
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
