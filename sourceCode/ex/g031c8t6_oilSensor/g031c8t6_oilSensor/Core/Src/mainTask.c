/*
 * mainTask.c
 *
 *  Created on: Aug 20, 2022
 *      Author: icepe
 */


#include "stm32g0xx_hal.h"
#include "NTC.h"
#include <string.h>
#include <math.h>
#include "SimpleKalmanFilter.h"
#include "define.h"
#include "EMA.h"
#include "command.h"
#include "flash.h"
//////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdio.h>
#include "MadgwickAHRS.h"
#include "strings.h"
#include "drv_spi.h"
#include "gpio.h"
#include "lsm6ds3.h"

#include "xprintf.h"
///////////////////////////////////////////////////////////////
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;

extern float roll;
extern float pitch;
extern float yaw;

static void com1Update(void);
static void handlerSensorData(void);
static void displaySensorData(void);
static void getADC_val(void);
static void setup(void);


typedef struct {
	__IO uint16_t adc_val[2];
	uint32_t T;
}TEMPT_t;

TEMPT_t internal_temp = {
		.T =0
};

typedef struct {
	uint32_t sum;
	uint8_t idx;
	uint8_t preiod;
}AVG_T;

AVG_T sensor_avg ={
		.sum = 0,
		.idx = 0,
		.preiod = 20
};

SENSOR_CONFIG_T sensorConfig;

SENSOR_ATTR_T sensorAttr ={
		.freq = 0,
		.last_freq = 0,
		.ema_freq = 0,
		.last_ema_freq = 0,
		.dif =0,
		.cut_off =0,
		.NF = 0,
		.N =0,
		.temprature = 0,
		.lastRasie =0,
		.errors = NO_ERR

};


APP_CONFIG_T g_appConfig ={
		.SecCount =0,
		.miliCount = 0,
		.tickCount =0,

};

INPUT_CAPTUTRE_T sensorCapture = {
				.IC1_VAL = 0,
				.IC2_VAL = 0,
				.isFirstCap = 0,
				.diffirent = 0,
				.freq = 0,
				.freq_idx = 0,
				.isFill = 0,
				.freq_update = 0,
				.time = 0

};


COMM_T com1 = {
				.len = 0 };

volatile uint32_t adcVal[2] ={0};


uint16_t cmd_len = 0;
uint8_t cmd_buf[RX_BUF_LEN] = { 0 };

uint16_t capture_compare = 0;
int internalTemp = 0;
float externalTemp =0;

int IsFirstCap = 0;
uint32_t IC1_VAL=0, IC2_VAL = 0, diffirent =0 ;
uint32_t freq =0;
uint32_t FREQ_ARR[MAX_FREQ_AVG] ={0};
uint32_t freq_total01 =0,freq_total02 =0;

uint32_t time_=0;


int mainTask(void) {
	setup();

	while (1) {
		if(((uint32_t)(g_appConfig.miliCount - time_)) > 100){
			Lsm6ds3_printRotationAngles();
			time_ = g_appConfig.miliCount;
		}

///////////////////////////////////////////////////
#if USING_EMA
		com1Update();
		if (g_appConfig.wirteConfigNow) {
			g_appConfig.wirteConfigNow = 0;
			printf("SKS_OIL_SENSOR wirte now\r\n");
			write_config();
		}

		handlerSensorData();
		if ((uint32_t) (g_appConfig.miliCount - g_appConfig.lastRaise)
				>= sensorConfig.update_freq) {
			g_appConfig.lastRaise = g_appConfig.miliCount;
			displaySensorData();
			/*getADC_val();
			 externalTemp = tempNTC(adcVal[1]);*/
			//printf("cur_f = %d\tEMA5 = %d\tEMA10 = %d\tt = %d\r\n",oilSensor.freq,(uint32_t)SensorEMA5.lastEMA,(uint32_t)SensorEMA10.lastEMA,(uint32_t)externalTemp);
		}
#endif
	}
	return 0;
}

static void setup(void){
	MX_GPIO_Init();
	SPI2_LowLevel_Init();
	SPIx_Init(SPI2);

//	xdev_out(HAL_UART_Transmit(&huart1, pData, Size, Timeout));

	/* USER CODE BEGIN 2 */
	HAL_Delay(1000);
//	printf("Wait install madgwick\r\n");
	xprintf("Wait install madgwick\r\n");
	Madgwick_init(SAMPLE_RATE);
	printf("Install Madgwick successfully\r\n");
	HAL_Delay(1000);
	/* Initialize mems driver interface */
	printf("Wait install LSM6DS3\r\n");
	if (!Lsm6ds3_int()) {
		printf("LSM6DS3 installation failed\r\n");
		while (1);
	}
	printf("Install LSM6DS3 successfully\r\n");
	HAL_Delay(1000);
////////////////////////////////////////////////////////
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) com1.buf,
	RX_BUF_LEN);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	load_config();
}


static void getADC_val(void){
#if 0
	ADC_select_internal_ntc();
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	adcVal[0] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	HAL_Delay(10);

	ADC_select_external_ntc();
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	adcVal[1] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	HAL_Delay(10);
#else
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	adcVal[1] = HAL_ADC_GetValue(&hadc1);
#endif

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	if (TIM2 == htim->Instance)
	{
		if (HAL_TIM_ACTIVE_CHANNEL_4 == htim->Channel) {
					if (!sensorCapture.isFirstCap) {
						sensorCapture.IC1_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
						sensorCapture.isFirstCap = 1;
					} else {
						sensorCapture.IC2_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
						sensorCapture.diffirent = (uint32_t) (sensorCapture.IC2_VAL - sensorCapture.IC1_VAL);
						sensorCapture.freq = TIM2_FREQ / sensorCapture.diffirent;
						sensorCapture.freq_update =1;

						__HAL_TIM_SET_COUNTER(&htim2, 0);
						sensorCapture.isFirstCap = 0;
					}
				}
	}
}

static void com1Update(void) {
	if (com1.len) {
		cmd_len = com1.len;
		com1.len = 0;
		memset(cmd_buf, 0, RX_BUF_LEN);
		memcpy(cmd_buf, com1.buf, cmd_len);
		server_on_command(1, cmd_buf, cmd_len);
	}
}

static void handlerSensorData(void) {
	uint32_t tmp_sensor = 0;

	if (sensorCapture.freq_update) { //pick up data
		sensorCapture.freq_update = 0;
		if (!sensorCapture.freq) {
			sensorAttr.errors = UNDER_FULL_FREQ;
			return;
		}
		if ((sensor_avg.idx < sensor_avg.preiod)) {
			sensor_avg.sum += sensorCapture.freq;
			sensor_avg.idx++;
			return;
		} else {
			sensorCapture.time++;
			tmp_sensor = sensor_avg.sum / sensor_avg.preiod;
			addStock((float) tmp_sensor, &SensorEMA10);
			sensor_avg.sum = 0;
			sensor_avg.idx = 0;
			//}
			//if ((uint32_t) (g_appConfig.miliCount - sensorAttr.lastRasie) > 100)
			//{

			sensorAttr.lastRasie = g_appConfig.miliCount;
			if (tmp_sensor < (uint32_t) (sensorConfig.full * 0.9)) {
				sensorAttr.errors = UNDER_FULL_FREQ;
				return;
			}
			if (tmp_sensor > (uint32_t) (sensorConfig.empty * 1.1)) {
				sensorAttr.errors = OVER_EMPTY_FREQ;
				return;
			}

			sensorAttr.errors = NO_ERR;

			if (sensorCapture.freq > sensorConfig.empty) {
				sensorAttr.freq = sensorConfig.empty;
			} else if (sensorCapture.freq < sensorConfig.full) {
				sensorAttr.freq = sensorConfig.full;
			} else {
				sensorAttr.freq = tmp_sensor;
			}

			//sensorAttr.freq = sensorCapture.freq;

			if (sensorAttr.freq >= sensorAttr.last_freq)
				sensorAttr.dif = sensorAttr.freq
						- (uint32_t) sensorAttr.last_ema_freq;
			else
				sensorAttr.dif = (uint32_t) sensorAttr.last_ema_freq
						- sensorAttr.freq;

			if ((sensorAttr.dif > (uint32_t) (sensorAttr.last_freq / 10))
					&& (sensorAttr.cut_off < 10)
					&& (sensorAttr.last_freq != 0)) {
				//skip
				sensorAttr.cut_off++;
				return;
			} else {
				//use this data
				addStock(sensorAttr.freq, &SensorEMA5);
				sensorAttr.cut_off = 0;
				sensorAttr.last_freq = sensorAttr.freq;
			}

			sensorAttr.ema_freq = getAverage();

			if (sensorAttr.ema_freq >= sensorConfig.empty) {

				sensorAttr.ema_freq = sensorConfig.empty;
				sensorAttr.NF = 0;
				sensorAttr.N = 0;

			} else if (sensorAttr.ema_freq <= sensorConfig.full) {

				sensorAttr.ema_freq = sensorConfig.full;
				sensorAttr.NF = 1.0;
				sensorAttr.N = 1023;
			} else {

				double Te = (double) (1.0 / (double) sensorConfig.empty);
				double Tf = (double) (1.0 / (double) sensorConfig.full);
				double Tx = (double) (1.0 / sensorAttr.ema_freq);
				sensorAttr.NF = (double) ((Tx - Te) / (Tf - Te));
				sensorAttr.N = (uint16_t) (sensorAttr.NF * 1023);
			}

		}
	}

}

char buf__[100] = {0};
static void displaySensorData(void) {

#if  SKS_FORM
//	printf("F=%x AF=%x XF=%x SF=%u N=%04x.0 XN=%04x.0 AN=%04x.0 CN=%04x.0 MN=%04x.0 SN=%04x.0 x=%0.0f y=%0.0f z=%0.0f idx3D=%d c3D=%d dT=%d tO=%d fM=%d dX=%d dY=%d st=%d v=%d\r\n",
//			sensorCapture.freq, ((uint32_t)(sensorAttr.ema_freq)), 0, 0, sensorAttr.N, 0, 0, 0, 0, 0,
//							roll,pitch,yaw,
//							0, 0, 0, 0, 0, 0, 0, 0,
//							0);

	printf("F=%x EF=%x F15=%x N=%04x.0 XN=%04x.0 AN=%04x.0 CN=%04x.0 MN=%04x.0 SN=%04x.0 x=%0.0f y=%0.0f z=%0.0f T=%d E=%x FU=%x EM=%x fM=%d\r\n", sensorCapture.freq, ((uint32_t)sensorAttr.ema_freq), 0, sensorAttr.N, 0, 0, 0, 0, 0,
			roll,pitch,yaw,
			0, 0, sensorConfig.full, sensorConfig.empty, 0);

//	printf("AF=%f\r\n", sensorAttr.ema_freq);

#endif

//	printf("F = %d AF = %d NF = %f N = %d  SF = %d SE = %d t = %d\r\n",
//			sensorCapture.freq, (uint32_t) sensorAttr.ema_freq,sensorAttr.NF,
//			sensorAttr.N, sensorConfig.full, sensorConfig.empty, 0);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (USART1 == huart->Instance) {
		com1.len = Size;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) com1.buf,
		RX_BUF_LEN);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	}
}


