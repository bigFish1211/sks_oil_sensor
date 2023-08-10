
#include "common.h"
#include "dvr_flash.h"
#include  "dvr_uart.h"
#include "ymodem.h"
#include "global.h"


#define ENABLE_PROTECT_CODE			0

pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask1 = 0;
__IO uint32_t FlashProtection = 0;
extern SKS_BOOT_INFO boot_inf;
void mainTask(void) {

#if ENABLE_PROTECT_CODE
	if (FLASH_GetReadOutProtectionStatus() != RDP_LEVEL_1) {
			//xprintf("Self Protect");
			FLASH_Unlock();
			FLASH_OB_Unlock();
			FLASH_ReadOutProtection(RDP_LEVEL_1);
			FLASH->CR |= FLASH_CR_OBL_LAUNCH;
			FLASH_OB_Lock();
			//unsigned long uTick = ulTickCount;
			while ((ulTickCount - uTick) < 10)
				;
			NVIC_SystemReset();
		}
		FLASH_OB_Lock();
#endif

	void mainTask(void) {
		uint8_t key = 0;
		uint8_t chr = 0;
		uint8_t boot = 0;
		uint8_t image_ok = 0;
		uint8_t istimeout = 0;

		while (1) {
			if (istimeout == 0) {
				if (FlashProtection != 0) {

				}
			}
			key = Rcv_Byte(&chr, 3);
			if (!key) {
				istimeout = 0;
				boot = 0;
				key = chr;
				if (key == 0x31) {
					/* Download user application in the Flash */
					//SerialDownload();
				} else if (key == 0x32) {
					/* Upload user application from the Flash */
					//SerialUpload();
				} else if ((key == 0x33) || (key == 'b') || (key == 'B')) {
					boot = 1;
				} else if ((key == 0x34) && (FlashProtection == 1)) {
					/* Disable the write protection of desired pages */
					//FLASH_DisableWriteProtectionPages(UserMemoryMask1);
				} else if ((key == SOH) || (key == STX)) {
					/* Download user application in the Flash */
					SerialDownload();
				} else if ((key == 'd') || (key == 'D')) {
					/* Download user application in the Flash */
					SerialDownload();
				}
				if (boot == 0)
					continue;
			} else {
				SerialPutString("Time out, booting ... ");
				istimeout = 1;
				boot = 1; //TimeOut
			}

			if (boot) {
				//image_ok = Verify
				printf("VERIFY IMAGE ...");
				image_ok = flash_verify_image(&boot_inf);
				//PrintBootInfo();
				if (image_ok > 0) {
					SerialPutString("SUCCESS!!!\r\n");
					/* Test if user code is programmed starting from address "ApplicationAddress" */
					if (((*(__IO uint32_t*) ApplicationAddress ) & 0x2FFE0000) == 0x20000000) {
						USART_Cmd(USART1, ENABLE);
						printf("BOOTING...\r\n");
						/*DISABLE ALL INTERUPT BEFORE JUMP TO APPLICATION*/
						/* TIM2 enable counter */
						unsigned long now = ulMiliCount;
						while ((ulMiliCount - now) < 10) {
						};
						TIM_Cmd(TIM2, DISABLE);
						USART_Cmd(USART1, DISABLE);

						SysTick->CTRL &= DISABLE;
						//SysTick_ITConfig(DISABLE);
						/* Jump to user application */
						JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
						Jump_To_Application = (pFunction) JumpAddress;
						/* Initialize user application's Stack Pointer */
						__set_MSP(*(__IO uint32_t*) ApplicationAddress);
						Jump_To_Application();

						SerialPutString("INVALID IMAGE SIGN, RETURN BOOT LOADER\r\n");
					}
				} else
					SerialPutString("FAILURE, RETURN BOOT LOADER\r\n");
			}
			blink = 1;
		}
	}
}
