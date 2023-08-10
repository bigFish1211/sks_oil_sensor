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
#include "main.h"
#include "stm32g031xx.h"
#include "crc.h"
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
#if 0
	uint32_t useroptionbyte = 0, WRPR = 0;
	uint16_t var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
	FLASH_Status status = FLASH_BUSY;

	WRPR = FLASH_GetWriteProtectionOptionByte();

	/* Test if user memory is write protected */
	if ((WRPR & UserMemoryMask) != UserMemoryMask) {
		useroptionbyte = FLASH_GetUserOptionByte();

		UserMemoryMask |= WRPR;

		status = FLASH_EraseOptionBytes();

		if (UserMemoryMask != 0xFFFFFFFF) {
			status = FLASH_EnableWriteProtection((uint32_t) ~UserMemoryMask);
		}

		/* Test if user Option Bytes are programmed */
		if ((useroptionbyte & 0x07) != 0x07) {
			/* Restore user Option Bytes */
			if ((useroptionbyte & 0x01) == 0x0) {
				var1 = OB_IWDG_HW;
			}
			if ((useroptionbyte & 0x02) == 0x0) {
				var2 = OB_STOP_RST;
			}
			if ((useroptionbyte & 0x04) == 0x0) {
				var3 = OB_STDBY_RST;
			}

			FLASH_UserOptionByteConfig(var1, var2, var3);
		}

		if (status == FLASH_COMPLETE) {
			//printf("Write Protection disabled...\r\n");

			//printf("...and a System Reset will be generated to re-load the new option bytes\r\n");

			/* Generate System Reset to load the new option byte values */
			NVIC_SystemReset();
		} else {
			//printf("Error: Flash write unprotection failed...\r\n");
		}
	} else {
		//printf("Flash memory not write protected\r\n");
	}
#endif
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
   //volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	//int *pInt = (int *)UBOOT_CONFIG_ADDRESS;
	flashClearFlag(
			FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR
					| FLASH_FLAG_WRPERR);
	flashEraserPage(UBOOT_CONFIG_ADDRESS); //CONFIG_MAX_2K
	uint32_t *pSource = boot_info;
	for (int i = 0; i < sizeof(SKS_BOOT_INFO) / 4; i++)
		FLASHStatus = FLASH_ProgramWord(UBOOT_CONFIG_ADDRESS + i * 4,
				pSource[i]);
}

int flash_copy_image(SKS_BOOT_INFO *boot_info) {
	//FLASH_Lock();
	uint32_t *data; // = (uint32_t*)BOOT_SECONDARY_ADDRESS;
	uint32_t count = 0;
	uint32_t tmp = (boot_info->SecondarySize / 4) + 1;
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

	data = BOOT_SECONDARY_ADDRESS;
	FLASH_ClearFlag(
			FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR
					| FLASH_FLAG_WRPERR);

	//FLASH_ErasePage(FLASH_CONFIG_ADDRESS+FLASH_PAGE_SIZE);
	printf("\r\n");
	printf("ERASE PRIMARY FLASH");
	for (count = 0; count < BOOT_IMAGE_PAGES; count++) {
		FLASH_ErasePage(BOOT_PRIMARY_ADDRESS + count * PAGE_SIZE); //CONFIG_MAX_2K
	}
	//&& (FLASHStatus == FLASH_COMPLETE)
	printf("\r\n");
	printf("COPY TO PRIMARY FLASH ");
	for (count = 0; count < tmp; count++) {
		if ((count % (tmp / 10)) == 0)
			printf(".");
		FLASHStatus = FLASH_ProgramWord(
				BOOT_PRIMARY_ADDRESS + count * sizeof(uint32_t), data[count]);
	}
	printf(". DONE\r\n");
	//printf("DONE\r\n BOOT_INFO\r\n");
	boot_info->PrimaryCRC = boot_info->SecondaryCRC;
	boot_info->PrimarySize = boot_info->SecondarySize;
	boot_info->PrimaryAddress = BOOT_PRIMARY_ADDRESS;
	boot_info->SecondaryAddress = BOOT_SECONDARY_ADDRESS;
	return 1;
}

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
