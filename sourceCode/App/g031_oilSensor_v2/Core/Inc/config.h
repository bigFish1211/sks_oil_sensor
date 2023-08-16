/*
 * config.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Thangnguyen
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>
#include <stdint.h>

#define USING_BOOTLOADER			1
#define APPLICATION_ADDRESS     	0x08002800

#define SKS_CONFIG_RECORD_SIZE		256
#define SF_CONFIG_SIGN				'd'

#define VERSION_NO "3.0.24"

#define APP_VERSION "LLS"VERSION_NO"F0-20221007"

#define SKS_ENABLE_GYRO 			1

#define SKS_DEBUG_NOW 				0

#define SKS_DEBUG_AVG_FREQUENCY		0

#define SKS_TRUNC_ERROR_SEC			5   //bo so giay bi loi

#define SKS_MEM_DEBUG				0
/*
 * Version LLS2.2.3.0F0-20220404:
 * 		- Chinh sua phan WHO_IM_I de support cac model Accelo.. khac nhau
 * Version LLS1.0.0.1SS-20151215:
 * 		- Ban khoi dau
 * Version LLS1.0.0.3SS-20161201:
 * 		- Sua phan loc dau, giam thoi gian lay trung binh
 * Version LLS2.0.0.1SS-20191125
 *		- Them phan accelerometer
 * Version LLS2.2.2.3SS-20200220
 *		- Tinh dau theo cam bien 3D
 * Version LLS2.2.2.4SS-20200220
 *		- Toi uu them phan tinh dau 3D
 * Version LLS2.2.2.5SS-20200320
 *      - Tinh dau nhieu che do, va co chon ouput mode
 *
 * */

typedef struct _ACC_GYRO_OFFSET{

	int16_t accX_offset;
	int16_t accY_offset;
	int16_t accZ_offset;

	int16_t gyroX_offset;
	int16_t gyroY_offset;
	int16_t gyroZ_offset;

}_ACC_GYRO_OFFSET;


#endif /* CONFIG_H_ */


