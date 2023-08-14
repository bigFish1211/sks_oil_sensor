/*
 * dvr_flash.c
 *
 *  Created on: Jan 3, 2023
 *      Author: icepe
 */
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "dvr_flash.h"
#include "common.h"
#include "stm32g031xx.h"

#define FLASH_PAGE_SIZE				0x800
void FLASH_Lock(void) {
	FLASH->CR |= FLASH_CR_LOCK;
}

void FLASH_Unlock(void) {
	if (FLASH->CR & FLASH_CR_LOCK) {
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
	}
}

void FLASH_OB_Unlock(void)
{

  if (FLASH->CR & FLASH_CR_OPTLOCK )
  {
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR = FLASH_OPTKEY2;
  }

}

void FLASH_OB_Lock(void)
{

	FLASH->CR |= FLASH_CR_OPTLOCK;

}

/**
 * @brief  Returns the FLASH Bank1 Status.
 * @note   This function can be used for all STM32F10x devices, it is equivalent
 *   to FLASH_GetStatus function.
 * @param  None
 * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *   FLASH_ERROR_WRP or FLASH_COMPLETE
 */

FLASH_Status FLASH_GetBankStatus(void) {

	uint32_t error = 0;
	FLASH_Status flashstatus = FLASH_COMPLETE;
#if defined(FLASH_DBANK_SUPPORT)
	error = (FLASH_SR_BSY1 | FLASH_SR_BSY2);
#else
  error = FLASH_SR_BSY1;
#endif

	if ((FLASH->SR & error) == error) {
		flashstatus = FLASH_BUSY;
	} else {
		if ((FLASH->SR & FLASH_SR_PROGERR) != 0) {
			flashstatus = FLASH_ERROR_PG;
		} else {
			if ((FLASH->SR & FLASH_SR_WRPERR) != 0) {
				flashstatus = FLASH_ERROR_WRP;
			} else {
				flashstatus = FLASH_COMPLETE;
			}
		}
	}

	return flashstatus;
}


FLASH_Status FLASH_GetBank1Status(void) {

	FLASH_Status flashstatus = FLASH_COMPLETE;

	if ((FLASH->SR & FLASH_FLAG_BANK1_BSY) == FLASH_FLAG_BSY) {
		flashstatus = FLASH_BUSY;
	} else {
		if ((FLASH->SR & FLASH_FLAG_BANK1_PROGERR) != 0) {
			flashstatus = FLASH_ERROR_PG;
		} else {
			if ((FLASH->SR & FLASH_FLAG_BANK1_WRPRTERR) != 0) {
				flashstatus = FLASH_ERROR_WRP;
			} else {
				flashstatus = FLASH_COMPLETE;
			}
		}
	}

	return flashstatus;
}

FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout) {
	FLASH_Status status = FLASH_COMPLETE;

	/* Check for the Flash Status */
	status = FLASH_GetBank1Status();
	/* Wait for a Flash operation to complete or a TIMEOUT to occur */
	while ((status == FLASH_FLAG_BANK1_BSY) && (Timeout != 0x00)) {
		status = FLASH_GetBank1Status();
		Timeout--;
	}
	if (Timeout == 0x00) {
		status = FLASH_TIMEOUT;
	}
	/* Return the operation status */
	return status;
}


FLASH_Status FLASH_WaitForLastBankOperation(uint32_t Timeout) {
	FLASH_Status status = FLASH_COMPLETE;
	uint32_t error = 0;
#if defined(FLASH_DBANK_SUPPORT)
	error = (FLASH_SR_BSY1 | FLASH_SR_BSY2);
#else
  error = FLASH_SR_BSY1;
#endif /* FLASH_DBANK_SUPPOR
	/* Check for the Flash Status */
	status = FLASH_GetBankStatus();
	/* Wait for a Flash operation to complete or a TIMEOUT to occur */
	while ((status == error) && (Timeout != 0x00)) {
		status = FLASH_GetBankStatus();
		Timeout--;
	}
	if (Timeout == 0x00) {
		status = FLASH_TIMEOUT;
	}
	/* Return the operation status */
	return status;

}

FLASH_Status FLASH_ErasePage(uint32_t Page_Address) {
	FLASH_Status status = FLASH_COMPLETE;
	uint32_t pageNum = (Page_Address-0x8000000)/0x800;
	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR |= FLASH_CR_PER; //Page Erase Set
	FLASH->CR |= (FLASH_CR_PNB & (pageNum << 3));
	FLASH->CR |= FLASH_CR_STRT; //Start Page Erase
	while ((FLASH->SR & FLASH_SR_BSY1));
	FLASH->CR &= ~FLASH_SR_BSY1;
	FLASH->CR &= ~FLASH_CR_PER; //Page Erase Clear
	return status;
}

FLASH_Status FLASH_EraseAllPages(void) {
	FLASH_Status status = FLASH_COMPLETE;
	return status;
}


void flashProgramDoubleWorld(uint32_t addr, uint64_t data) {
}

FLASH_Status FLASH_ProgramDoubleWord(uint32_t addr, uint64_t data) {

	FLASH_Status status = FLASH_COMPLETE;
	status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

	FLASH->CR |= FLASH_CR_PG;/*!< Programming */
	*(uint32_t*) addr = (uint32_t) data;
	__ISB();
	*(uint32_t*) (addr + 4U) = (uint32_t) (data >> 32U);

	status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

	FLASH->CR &= ~FLASH_CR_PG;

	return status;
}


void FLASH_ClearFlag(uint32_t FLASH_FLAG) {
	FLASH->SR = FLASH_FLAG;
}

int FlashWritePage(uint32_t addr, uint8_t *pdata, uint16_t len) {

	uint32_t pageAddr = addr;
	uint64_t *pRecord = (uint64_t*) pdata;

	FLASH_Unlock();
	FLASH_ErasePage(pageAddr);

	for (int i = 0; i < len; i += 8, pRecord++, pageAddr += 8) {
		if (!FLASH_ProgramDoubleWord(pageAddr, *pRecord)) {
			//if (!FLASH_ProgramDoubleWord(pageAddr,(uint64_t)0xaa)) {
			FLASH_Lock();
			return 0;
		}
		if (*(uint64_t*) pageAddr != *pRecord) {

		}
	}
	FLASH_Lock();
	return 1;
}

void FLASH_ReadOutProtection(uint32_t RDPLevel){
/*	uint32_t optr;
	optr =  FLASH->OPTR;
	optr  &= ~ FLASH_OPTR_RDP;
	// FLASH->OPTR &= ~ FLASH_OPTR_RDP;
	 FLASH->OPTR |= (optr|RCC);
	// FLASH->CR |= FLASH_CR_OBL_LAUNCH;
	FLASH->CR |= FLASH_CR_OPTSTRT;
	//FLASH_WaitForLastOperation(ProgramTimeout);
	FLASH_WaitForLastBankOperation(ProgramTimeout);
	FLASH->CR &= ~FLASH_CR_OPTSTRT;*/
}
uint32_t FLASH_GetReadOutProtectionStatus(void) {
	/*uint32_t rdplvl = FLASH->OPTR & FLASH_OPTR_RDP;
	if ((rdplvl != RDP_LEVEL_0) && (rdplvl != RDP_LEVEL_2)) {
		return (RDP_LEVEL_1);
	} else {
		return rdplvl;
	}*/
}
