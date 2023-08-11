#include "common.h"
#include "dvr_flash.h"
#include  "dvr_uart.h"
#include "ymodem.h"
#include "global.h"
#include "system_g031.h"

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
		key = Rcv_Byte(&chr, 5);
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
			xprintf("Time out, booting ... ");
			istimeout = 1;
			boot = 1; //TimeOut
		}

		if (boot) {
			//image_ok = Verify
			printf("VERIFY IMAGE ...");
			image_ok = flash_verify_image(&boot_inf);
			//PrintBootInfo();
			if (image_ok > 0) {
				xprintf("SUCCESS!!!\r\n");
				if (((*(__IO uint32_t*) ApplicationAddress ) & 0x2FFE0000) == 0x20000000) {
					xprintf("BOOTING...\r\n");
					/*DISABLE ALL INTERUPT BEFORE JUMP TO APPLICATION*/
					/* TIM2 enable counter */
					unsigned long now = ulMiliCount;
					while ((ulMiliCount - now) < 10) ;
					RCC_DeInit();
					SysTick->CTRL = 0;
					SysTick->LOAD = 0;
					SysTick->VAL = 0;
					__disable_irq();
					SYSCFG->CFGR1 = 0x01;
					Jump_To_Application = (void (*)(void)) (*((uint32_t*) (ApplicationAddress + 4)));
					Jump_To_Application();
					xprintf("INVALID IMAGE SIGN, RETURN BOOT LOADER\r\n");
				}
			} else
				xprintf("FAILURE, RETURN BOOT LOADER\r\n");
		}
	}

}

uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum){
  uint32_t i = 0, res = 0,val = 0;
  if ('0' == inputstr[0] && ( 'x' == inputstr[1] || 'X' == inputstr[1])){
    if (inputstr[2] == '\0'){
      return 0;
    }
    for (i = 2; i < 11; i++){
      if ('\0' == inputstr[i])
      {
        *intnum = val;
        res = 1;//retutn 1
        break;
      }
      if (ISVALIDHEX(inputstr[i])){
        val = (val << 4) + CONVERTHEX(inputstr[i]);
      }
      else{
        res = 0;  /* return 0, Invalid input */
        break;
      }
    }
    if (i >= 11){/* over 8 digit hex --invalid */
      res = 0;
    }
  }
  else{ /* max 10-digit decimal input */
    for (i = 0;i < 11;i++){
      if ('\0' == inputstr[i]){
        *intnum = val;
        res = 1; /* return 1 */
        break;
      }
      else if (('k' == inputstr[i] || 'K' == inputstr[i]) && (i > 0)){
        val = val << 10;
        *intnum = val;
        res = 1;
        break;
      }
      else if (('m' == inputstr[i] || 'M' == inputstr[i]) && (i > 0)){
        val = val << 20;
        *intnum = val;
        res = 1;
        break;
      }
      else if (ISVALIDDEC(inputstr[i])){
        val = val * 10 + CONVERTDEC(inputstr[i]);
      }
      else{
        res = 0; /* return 0, Invalid input */
        break;
      }
    }
    if (i >= 11){/* Over 10 digit decimal --invalid */
      res = 0;
    }
  }

  return res;
}
