/*
 * EEprom.h
 *
 *  Created on: Feb 16, 2017
 *      Author: ThangNguyen
 */

#ifndef FLASH_H_
#define FLASH_H_
#include <stdint.h>
/*
 */
#define FLASH_PAGE_SIZE    				((uint16_t)0x400)
#define BANK1_WRITE_START_ADDR  		((uint32_t)0x0800f800)
//#define BANK1_WRITE_START_ADDR  		((uint32_t)0x0800f000)//page61
#define BANK1_WRITE_END_ADDR    		((uint32_t)0x0800ffff)
#define NUM_CONFIG_PAGE 				1
#define BANK1_CONFIG_START_ADDR 		BANK1_WRITE_START_ADDR
#define BANK1_CONFIG_END_ADDR 			(uint32_t)(BANK1_WRITE_START_ADDR+FLASH_PAGE_SIZE*NUM_CONFIG_PAGE)

void FlashInit(void);
void flash_write_config(uint32_t address, char *pdata, int len);
void flash_read_config(uint32_t address, char *pdata, int len);
void write_config(void);
void load_config(void);
#endif /* EEPROM_H_ */
