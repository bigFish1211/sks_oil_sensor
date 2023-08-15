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

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;

static void com1Update(void);
static void handlerSensorData(void);
static void displaySensorData(void);
static void getADC_val(void);
static void setup(void);


SENSOR_CONFIG_T sensorConfig;

SENSOR_ATTR_T sensorAttr ={
		.freq = 0,
		.last_freq = 0,
		.ema_freq = 0,
		.last_ema_freq = 0,
		.dif =0,
		.cut_off =0,
		.NF = 0,
		.N =0
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
				.freq_update = 0

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


int mainTask(void) {
	setup();

	while (1) {
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

					//	__HAL_TIM_SET_COUNTER(&htim2, 0);
						TIM2->CNT= 0;
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

	if (sensorCapture.freq_update) { //pick up data
		sensorCapture.freq_update = 0;
		addStock(sensorAttr.freq, &SensorEMA10);
		if (sensorCapture.freq > sensorConfig.empty) {
			sensorAttr.freq = sensorConfig.empty;
		} else if (sensorCapture.freq < sensorConfig.full) {
			sensorAttr.freq = sensorConfig.full;
		} else {
			sensorAttr.freq = sensorCapture.freq;
		}

		sensorAttr.freq = sensorCapture.freq;
#if 1
		if (sensorAttr.freq >= sensorAttr.last_freq)
			sensorAttr.dif = sensorAttr.freq - (uint32_t)sensorAttr.last_ema_freq;
		else
			sensorAttr.dif = (uint32_t)sensorAttr.last_ema_freq - sensorAttr.freq;

#else
		sensorAttr.dif =abs((int32_t)sensorAttr.freq -  (int32_t)( sensorAttr.last_ema_freq));
#endif

		if ((sensorAttr.dif > (uint32_t) (sensorAttr.last_freq / 10))
				&& (sensorAttr.cut_off < 10) && (sensorAttr.last_freq != 0)) {
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
		if (sensorAttr.ema_freq > sensorConfig.empty)
			sensorAttr.ema_freq = sensorConfig.empty;
		if (sensorAttr.ema_freq < sensorConfig.full)
			sensorAttr.ema_freq = sensorConfig.full;

		sensorAttr.NF = (double) (((double) sensorConfig.empty
				- sensorAttr.ema_freq) / ((double) sensorConfig.empty)
				- (double) sensorConfig.full);
		sensorAttr.N = (uint16_t) (sensorAttr.NF * 1023);

	}

}

static void displaySensorData(void) {

#if  SKS_FORM
	printf("F=%x AF=%x XF=%x SF=%u N=%04x.0 XN=%04x.0 AN=%04x.0 CN=%04x.0 MN=%04x.0 SN=%04x.0 x=%d y=%d z=%d idx3D=%d c3D=%d dT=%d tO=%d fM=%d dX=%d dY=%d st=%d v=%d\r\n",
							oilSensor.freq, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0,0,0,
							0, 0, 0, 0, 0, 0, 0, 0,
							g_appStarts.Vibration_cnt);
#endif

	printf("F = %d AF = %d NF = %f N = %d  SF = %d SE = %d t = %d\r\n",
			sensorCapture.freq, (uint32_t) sensorAttr.ema_freq,sensorAttr.NF,
			sensorAttr.N, sensorConfig.full, sensorConfig.empty, 0);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (USART1 == huart->Instance) {
		com1.len = Size;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) com1.buf,
		RX_BUF_LEN);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	}
}


