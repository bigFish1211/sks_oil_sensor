/**
 ******************************************************************************
 * @file    IAP/inc/common.h
 * @author  MCD Application Team
 * @version V3.3.0
 * @date    10/15/2010
 * @brief   This file provides all the headers of the common functions.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _COMMON_H
#define _COMMON_H
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"

/* Exported types ------------------------------------------------------------*/
typedef void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE       128

//#define ApplicationAddress    0x8002800  //20KB 0x8005000, 10k = 0x8002800
#define ApplicationAddress   (uint32_t)0x8002800  //10
#define PAGE_SIZE                         (0x800)    /* 2 Kbyte */
//#define FLASH_SIZE                        (0x10000)  /* 64 KBytes */
/* Compute the FLASH upload image size */
//#define FLASH_IMAGE_SIZE                   (uint32_t) (FLASH_SIZE - (ApplicationAddress - 0x08000000))
/* Exported macro ------------------------------------------------------------*/
/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

//#if (FLASH_SIZE == 0x10000)
#define UBOOT_CONFIG_ADDRESS    		0x800F000   //Page 30
#define BOOT_IMAGE_PAGES	    		25  		//20KB - 1KB/page
#define BOOT_PRIMARY_ADDRESS    		ApplicationAddress							  //Page 255
#define BOOT_SECONDARY_ADDRESS    		(ApplicationAddress+BOOT_IMAGE_PAGES*PAGE_SIZE)  //Page 255
#define FLASH_IMAGE_SIZE                (uint32_t)(BOOT_IMAGE_PAGES*PAGE_SIZE)

//REPORT: PAGE 138

/*
 *  boot_info[0]: CRC of Primary ELF
 *  boot_info[1]: Size of Primary ELF
 *  boot_info[2]: CRC of Second ELF
 *  boot_info[3]: Size of Second ELF
 */
typedef struct _SKS_BOOT_INFO {
	uint32_t PrimaryCRC;
	uint32_t PrimarySize;
	uint32_t SecondaryCRC;
	uint32_t SecondarySize;
	uint32_t FirmwareUpdate;
	uint32_t PrimaryAddress;
	uint32_t SecondaryAddress;
	uint32_t ConfigAddress;
	uint32_t UpdateFirmware;
	uint32_t FirmwareVersion;
	char DATA[20];
	char pad[4];
} SKS_BOOT_INFO;
void flash_read_uboot_info(SKS_BOOT_INFO *boot_info);
void flash_write_uboot_info(SKS_BOOT_INFO *boot_info);
int flash_verify_image(SKS_BOOT_INFO *boot_info);
int flash_copy_image(SKS_BOOT_INFO *boot_info);

void PrintBootInfo();
//extern SKS_BOOT_INFO boot_info;
#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
