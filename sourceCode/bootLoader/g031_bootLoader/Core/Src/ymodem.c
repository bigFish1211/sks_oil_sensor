/**
 ******************************************************************************
 * @file    IAP/src/ymodem.c
 * @author  MCD Application Team
 * @version V3.3.0
 * @date    10/15/2010
 * @brief   This file provides all the software functions related to the ymodem
 *          protocol.
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
#include "common.h"
#include "ymodem.h"
#include "main.h"
#include "dvr_flash.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t file_name[FILE_NAME_LENGTH];
//uint32_t FlashDestination = ApplicationAddress; /* Flash user program offset */
uint16_t PageSize = PAGE_SIZE;
uint32_t EraseCounter = 0x0;
uint32_t NbrOfPage = 0;
FLASH_Status FLASHStatus = FLASH_COMPLETE;
uint32_t RamSource;
//extern uint8_t tab_1024[1024];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Receive byte from sender
 * @param  c: Character
 * @param  timeout: Timeout
 * @retval 0: Byte received
 *         -1: Timeout
 */
static int32_t Receive_Byte(uint8_t *c, uint32_t timeout) {
	while (timeout-- > 0) {
		if (SerialKeyPressed(c) == 1) {
			return 0;
		}
	}
	return -1;
}

/**
 * @brief  Send a byte
 * @param  c: Character
 * @retval 0: Byte sent
 */
static uint32_t Send_Byte1(uint8_t c) {
	SerialPutChar(c);
	return 0;
}

/**
 * @brief  Receive a packet from sender
 * @param  data
 * @param  length
 * @param  timeout
 *     0: end of transmission
 *    -1: abort by sender
 *    >0: packet length
 * @retval 0: normally return
 *        -1: timeout or packet error
 *         1: abort by user
 */
static int32_t Receive_Packet(uint8_t *data, int32_t *length, uint32_t timeout) {
	uint16_t i, packet_size;
	uint8_t c;
	*length = 0;
	if (Receive_Byte(&c, timeout) != 0) {
		return -1;
	}
	switch (c) {
	case SOH:
		packet_size = PACKET_SIZE;
		break;
	case STX:
		packet_size = PACKET_1K_SIZE;
		break;
	case EOT:
		return 0;
	case CA:
		if ((Receive_Byte(&c, timeout) == 0) && (c == CA)) {
			*length = -1;
			return 0;
		} else {
			return -1;
		}
	case ABORT1:
	case ABORT2:
		return 1;
	default:
		return -1;
	}
	*data = c;
	for (i = 1; i < (packet_size + PACKET_OVERHEAD); i++) {
		if (Receive_Byte(data + i, timeout) != 0) {
			return -1;
		}
	}
	if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
		return -1;
	}
	*length = packet_size;
	return 0;
}

/**
 * @brief  Receive a file using the ymodem protocol
 * @param  buf: Address of the first byte
 * @retval The size of the file
 */
uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
int32_t Ymodem_Receive() {
	uint8_t file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
	uint8_t buf_data[PACKET_1K_SIZE];
	int32_t i, j, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;

	/* Initialize FlashDestination variable */
	// uint32_t DestinationAddress = ApplicationAddress; /* Flash user program offset */
	uint32_t DestinationAddress = BOOT_SECONDARY_ADDRESS; /* Flash user program offset */
	uint32_t FlashDestination = DestinationAddress;
	FLASH_Unlock();
	for (session_done = 0, errors = 0, session_begin = 0;;) {
		for (packets_received = 0, file_done = 0;;) {
			switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT)) {
			case 0:
				errors = 0;
				switch (packet_length) {
				/* Abort by sender */
				case -1:
					SerialPutChar(ACK);
					return 0;
					/* End of transmission */
				case 0:
					SerialPutChar(ACK);
					file_done = 1;
					break;
					/* Normal packet */
				default:
					if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff)) {
						SerialPutChar(NAK);
					} else {
						if (packets_received == 0) {
							/* Filename packet */
							if (packet_data[PACKET_HEADER] != 0) {
								/* Filename packet has valid data */
								for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);) {
									file_name[i++] = *file_ptr++;
								}
								file_name[i++] = '\0';
								for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);) {
									file_size[i++] = *file_ptr++;
								}
								file_size[i++] = '\0';
								Str2Int(file_size, &size);

								/* Test the size of the image to be sent */
								/* Image size is greater than Flash size */
								if (size > (FLASH_SIZE - 1)) {
									/* End session */
									SerialPutChar(CA);
									SerialPutChar(CA);
									return -1;
								}

								/* Erase the needed pages where the user application will be loaded */
								/* Define the number of page to be erased */
								NbrOfPage = FLASH_PagesMask(size);

								/* Erase the FLASH pages */
								FLASH_Unlock();
								for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++) {
									FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
								}
								SerialPutChar(ACK);
								SerialPutChar(CRC16);
							}
							/* Filename packet is empty, end session */
							else {
								SerialPutChar(ACK);
								file_done = 1;
								session_done = 1;
								break;
							}
						}
						/* Data packet */
						else {
							//memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
							memcpy(buf_data, &packet_data[PACKET_HEADER], packet_length);
							RamSource = packet_data + PACKET_HEADER;
							uint64_t *pRecord = (uint64_t*) buf_data;
							for (j = 0; (j < packet_length) && (FlashDestination < DestinationAddress + size); j += 8) {
								FLASH_ProgramDoubleWord(FlashDestination, *pRecord);
								if (*(uint64_t*) FlashDestination != *pRecord) {
									//  End session
									SerialPutChar(CA);
									SerialPutChar(CA);
									return -2;
								}
								FlashDestination += 8;
								pRecord++;

							}
							SerialPutChar(ACK);

						}
						packets_received++;
						session_begin = 1;
					}
				}
				break;
			case 1:
				SerialPutChar(CA);
				SerialPutChar(CA);
				return -3;
			default:
				if (session_begin > 0) {
					errors++;
				}
				if (errors > MAX_ERRORS) {
					SerialPutChar(CA);
					SerialPutChar(CA);
					return 0;
				}
				SerialPutChar(CRC16);
				break;
			}
			if (file_done != 0) {
				break;
			}
		}
		if (session_done != 0) {
			break;
		}
	}
	return (int32_t) size;

}

/**
 * @brief  Update CRC16 for input byte
 * @param  CRC input value
 * @param  input byte
 * @retval None
 */
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte) {
	uint32_t crc = crcIn;
	uint32_t in = byte | 0x100;
	do {
		crc <<= 1;
		in <<= 1;
		if (in & 0x100)
			++crc;
		if (crc & 0x10000)
			crc ^= 0x1021;
	} while (!(in & 0x10000));
	return crc & 0xffffu;
}

/**
 * @brief  Cal CRC16 for YModem Packet
 * @param  data
 * @param  length
 * @retval None
 */
uint16_t Cal_CRC16(const uint8_t *data, uint32_t size) {
	uint32_t crc = 0;
	const uint8_t *dataEnd = data + size;
	while (data < dataEnd)
		crc = UpdateCRC16(crc, *data++);

	crc = UpdateCRC16(crc, 0);
	crc = UpdateCRC16(crc, 0);
	return crc & 0xffffu;
}

/**
 * @}
 */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
