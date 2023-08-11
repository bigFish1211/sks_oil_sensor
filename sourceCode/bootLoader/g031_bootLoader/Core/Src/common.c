/**
 ******************************************************************************
 * @file    IAP/src/common.c
 * @author  MCD Application Team
 * @version V3.3.0
 * @date    10/15/2010
 * @brief   This file provides all the common functions.
 ******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 */

/** @addtogroup IAP
 * @{
 */

/* Includes ------------------------------------------------------------------*/
//#include "common.h"
#include <dvr_flash.h>
#include "common.h"
#include "stm32g031xx.h"
#include "dvr_crc.h"
/**
 * @brief  Calculate the number of pages
 * @param  Size: The image size
 * @retval The number of pages
 */
uint32_t FLASH_PagesMask(__IO uint32_t Size) {
	uint32_t pagenumber = 0x0;
	uint32_t size = Size;

	if ((size % PAGE_SIZE) != 0) {
		pagenumber = (size / PAGE_SIZE) + 1;
	} else {
		pagenumber = size / PAGE_SIZE;
	}
	return pagenumber;

}

/**
 * @brief  Disable the write protection of desired pages
 * @param  None
 * @retval None
 */
void FLASH_DisableWriteProtectionPages(uint32_t UserMemoryMask) {
}

/**
 * @}
 */

/*
 * flash_verify_image
 * return value:
 * 	-1: No execute image
 * 	-2: No secondary image, primary image checksum failed
 */
#define FLASH_DEBUG 0
int flash_verify_image(SKS_BOOT_INFO *boot_info) {
	//char str [255] = {0};
	uint32_t crc_primary = 0;

	flash_read_uboot_info(boot_info);
	CRC_ResetDR();
	crc_primary = CRC_CalcBlockCRC((uint32_t) BOOT_PRIMARY_ADDRESS,
			(uint32_t) (boot_info->PrimarySize / 4) + 1);
	if (crc_primary != boot_info->PrimaryCRC) {
		return 0; //Invalid PRIMARY's  CRC
	}
	//printf("VERIFY DATA SUCCESS\r\n");
	//CRC->DR
	return 1;
}

void flash_read_uboot_info(SKS_BOOT_INFO *boot_info) {
	int i = 0;
	uint32_t *pInt = UBOOT_CONFIG_ADDRESS;
	uint32_t *pDest = boot_info;
	for (i = 0; i < sizeof(SKS_BOOT_INFO) / 4; i++) {
		//flash clear flag
	/*	FLASH_ClearFlag(
				FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR
						| FLASH_FLAG_WRPERR);*/
		pDest[i] = pInt[i];

	}
}

void flash_write_uboot_info(SKS_BOOT_INFO *boot_info) {
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
		//int *pInt = (int *)UBOOT_CONFIG_ADDRESS;
		FLASH->SR &= ~(FLASH_SR_EOP|FLASH_SR_WRPERR|FLASH_SR_PGAERR);
		FLASH_ErasePage(UBOOT_CONFIG_ADDRESS);  //CONFIG_MAX_2K

		uint64_t *pSource = (uint64_t*) boot_info;
		//boot_info->BootVersion = UBOOT_VERSION;
		for (int i = 0; i <  sizeof(SKS_BOOT_INFO) / 8; i++,pSource++)
			FLASHStatus = FLASH_ProgramDoubleWord(UBOOT_CONFIG_ADDRESS + i * 8, *pSource);
}

int flash_copy_image(SKS_BOOT_INFO *boot_info) {
	//FLASH_Lock();
	uint64_t *data;  // = (uint32_t*)BOOT_SECONDARY_ADDRESS;
	uint32_t count = 0;
	uint32_t tmp = (boot_info->SecondarySize / 8) + 1;
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

	data = BOOT_SECONDARY_ADDRESS;
	//FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PROGERR| FLASH_FLAG_WRPRTERR);
	FLASH->SR &= ~(FLASH_SR_EOP | FLASH_SR_WRPERR | FLASH_SR_PGAERR);
	//FLASH_ErasePage(FLASH_CONFIG_ADDRESS+FLASH_PAGE_SIZE);
	int maxPages = ((boot_info->SecondarySize & 0xFFFFF800) + 0x800) / 0x800;
	if (maxPages > 100)
		maxPages = 100;
	for (count = 0; count < maxPages; count++) {
		FLASH_ErasePage(BOOT_PRIMARY_ADDRESS + count * PAGE_SIZE); //CONFIG_MAX_2K
	}
	//&& (FLASHStatus == FLASH_COMPLETE)
	for (count = 0; count < tmp; count++) {
		if ((count % (tmp / 10)) == 0)
			SerialPutChar('.');
		FLASHStatus = FLASH_ProgramDoubleWord( BOOT_PRIMARY_ADDRESS + count * sizeof(uint64_t), data[count]);

	}
	//SerialPutString("DONE\r\n BOOT_INFO\r\n");
	boot_info->PrimaryCRC = boot_info->SecondaryCRC;
	boot_info->PrimarySize = boot_info->SecondarySize;
	boot_info->PrimaryAddress = BOOT_PRIMARY_ADDRESS;
	boot_info->SecondaryAddress = BOOT_SECONDARY_ADDRESS;

	return 1;
}

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
