/*
 * internal_temperature.c
 *
 *  Created on: Sep 19, 2022
 *      Author: icepe
 */

#include "internal_temperature.h"
#include "stm32f0xx.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_adc.h"
#include "define.h"

/* Configurations BEGIN */
#define TMPSENSOR_USE_INTREF 1 /* 1 - Use Internal Reference Voltage; 0 - Not use; */
/* Configurations END */

/* Constant values BEGIN */
#define TMPSENSOR_AVGSLOPE  	((uint32_t) 4000) /* mV/°C */
#define TMPSENSOR_V30  			((uint32_t) 1340)/* V (at 30 °C)  */

#define TMPSENSOR_ADCMAX 		((uint32_t) 3095) /* 12-bit ADC maximum value (12^2)-1)  */
#define TMPSENSOR_ADCREFVOL  	((uint32_t) 3300) /* Typical reference voltage, V  */
#define TMPSENSOR_ADCVREFINT	((uint32_t) 1200)/* Internal reference voltage, V  */
/* Constant values END */


static void ADC_Config(void);
static void DMA_Config(void);

__IO uint16_t ADC_val[NUMBER_OF_ADC_CHANNEL]={0};
void internalTempInit(void){
	ADC_Config();
	DMA_Config();
}

uint32_t getTemperature(void){
	float  adc_intref = ADC_val[1];
	float  adc_sensor = ADC_val[0];
#if(TMPSENSOR_USE_INTREF)

	float intref_vol = (TMPSENSOR_ADCMAX*TMPSENSOR_ADCVREFINT)/adc_intref;

#else
	float intref_vol = TMPSENSOR_ADCREFVOL;
#endif

	float sensor_vol = adc_sensor * intref_vol/TMPSENSOR_ADCMAX;

	uint32_t sensor_tmp = (uint32_t)((TMPSENSOR_V30 - sensor_vol)*1000/TMPSENSOR_AVGSLOPE + 30);

	return sensor_tmp;
}


static void ADC_Config(void)
{
  ADC_InitTypeDef     ADC_InitStructure;
  /* ADC1 DeInit */
  ADC_DeInit(ADC1);

   /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);

  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */
  ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_239_5Cycles);
  ADC_TempSensorCmd(ENABLE);

  /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */
  ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint , ADC_SampleTime_239_5Cycles);
  ADC_VrefintCmd(ENABLE);

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);

  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);

  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

  /* ADC1 regular Software Start Conv */
  ADC_StartOfConversion(ADC1);
}
static void DMA_Config(void)
{
  DMA_InitTypeDef   DMA_InitStructure;
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_val;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = NUMBER_OF_ADC_CHANNEL;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);

}
