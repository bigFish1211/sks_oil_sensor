/**
 ******************************************************************************
 * @file    IAP/src/download.c
 * @author  MCD Application Team
 * @version V3.3.0
 * @date    10/15/2010
 * @brief   This file provides the software which allows to download an image
 *          to internal Flash.
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
#include "ymodem.h"
#include "common.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SKS_BOOT_INFO boot_inf;
extern uint8_t file_name[FILE_NAME_LENGTH];
uint8_t tab_1024[1024] = { 0 };

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Download a file via serial port
 * @param  None
 * @retval None
 */
void SerialDownload(void) {
	int32_t Size = 0;

	//FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
#define REPORT_POINTER_ADDRESS			0x807F000  //Page 254
	//FLASH_ErasePage(REPORT_POINTER_ADDRESS);
	printf(
			"\r\nWaiting for the file to be sent ... (press 'a' to abort)\n\r");
	Size = Ymodem_Receive(&tab_1024[0]);
	if (Size > 0) {

		flash_read_uboot_info(&boot_inf);
		boot_inf.SecondarySize = Size;
	/*	FLASH_ClearFlag(
				FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR
						| FLASH_FLAG_WRPERR);*/
		CRC_ResetDR();
		//boot_inf.SecondaryCRC = CRC_CalcBlockCRC(BOOT_SECONDARY_ADDRESS, (uint32_t)(Size/4)+1);
		boot_inf.SecondaryCRC = CRC_CalcBlockCRC(BOOT_PRIMARY_ADDRESS,
				(uint32_t) (Size / 4) + 1);
		boot_inf.PrimaryCRC = boot_inf.SecondaryCRC;
		boot_inf.PrimarySize = boot_inf.SecondarySize;
		//PrintBootInfo();
		boot_inf.PrimaryAddress = BOOT_PRIMARY_ADDRESS;
		boot_inf.SecondaryAddress = BOOT_SECONDARY_ADDRESS;
		boot_inf.UpdateFirmware = 1;
		flash_write_uboot_info(&boot_inf);
		printf(
				"\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n");

	} else if (Size == -1) {
		printf(
				"\n\n\rThe image size is higher than the allowed space memory!\n\r");
	} else if (Size == -2) {
		printf("\n\n\rVerification failed!\n\r");
	} else if (Size == -3) {
		printf("\r\n\nAborted by user.\n\r");
	} else {
		printf("\n\rFailed to receive the file!\n\r");
	}
}

/**
 * @}
 */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
