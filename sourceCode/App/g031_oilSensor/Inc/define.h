/*
 * define.h
 *
 *  Created on: Feb 25, 2022
 *      Author: icepe
 */

#ifndef INC_DEFINE_H_
#define INC_DEFINE_H_

//#define HUB_GET_COMM
//#define HUB_TEST_SERIAL
//#define HUB_DEBUG_ENABLE
//#define TEST_FLASH

#define MAIN_PROGRAM				1

#define  FLASH_PROGRAM_WORD

#define MAX_EXTERNAL_COM			6
#define RX_BUF_LEN					2048
#define TX_BUF_LEN					128

#define UPLOAD_TIMEOUT				3//s
#define GET_PACKAGE_TIMEOUT			5//s

#define SKS_CONFIG_RECORD_SIZE		256
#define SF_CONFIG_SIGN				0x534B532A

#define PC_COMM_BAUD				19200
#define COMM_DEFAULT_BAUD			19200
#define COMM_MAX_TIMEOUT			100//ms


#endif /* INC_DEFINE_H_ */
