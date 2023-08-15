/*
 * define.h
 *
 *  Created on: Aug 23, 2022
 *      Author: icepe
 */

#ifndef INC_DEFINE_H_
#define INC_DEFINE_H_

#define RX_BUF_LEN					128
#define ADC_VREF					3300//mv
#define TIM2_FREQ					64000000UL//1000000UL
#define MAX_FREQ_AVG				128
#define USING_KALMAN				0
#define USING_EMA					1
#define CONFIG_SIGN					'*'
#define SKS_CONFIG_RECORD_SIZE		256
#define SKS_FORM					1
#define FAULT_DETECT				0

#include "stdio.h"

typedef struct{
	uint32_t IC1_VAL;
	uint32_t IC2_VAL;
	uint8_t isFirstCap;
	uint32_t diffirent;
	uint32_t freq;
	uint8_t isFill;
	uint32_t FREQ_ARR[MAX_FREQ_AVG];
	uint32_t freq_idx;
	uint8_t  freq_update;
	uint32_t time;

} INPUT_CAPTUTRE_T;

typedef struct{
	uint32_t miliCount;
	uint32_t tickCount;
	uint32_t SecCount;
	uint32_t lastRaise;
	uint16_t temperature;
	uint8_t  wirteConfigNow;
	uint8_t debug;
}APP_CONFIG_T;

typedef enum {
	EMA5 = 0,
	EMA10 = 1,
	EMA15 = 2,
	EMA30 = 3,
	EMA50 = 4,
} EMA_TYPE;

typedef enum {
	NO_ERR = 0,
	OVER_EMPTY_FREQ = 1,
	UNDER_FULL_FREQ = 2
}SENSOR_ERR_T;

typedef struct{
	uint8_t sign;
	EMA_TYPE ema;
	uint32_t update_freq;
	uint32_t full;
	uint32_t empty;
} SENSOR_CONFIG_T;

typedef struct {
	uint32_t freq;
	uint32_t last_freq;
	uint32_t dif;
	uint16_t cut_off;
	double ema_freq;
	double last_ema_freq;
	double NF;
	uint16_t N;
	uint32_t temprature;
	uint32_t lastRasie;
	SENSOR_ERR_T errors;
}SENSOR_ATTR_T;

typedef struct {
	volatile uint8_t buf[RX_BUF_LEN];
	volatile uint16_t len;
} COMM_T;

extern SENSOR_ATTR_T sensorAttr;
extern SENSOR_CONFIG_T sensorConfig;
extern COMM_T com1;
extern APP_CONFIG_T g_appConfig;
extern INPUT_CAPTUTRE_T sensorCapture;

#endif /* INC_DEFINE_H_ */
