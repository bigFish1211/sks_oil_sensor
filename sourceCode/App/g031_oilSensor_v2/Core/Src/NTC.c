/*
 * NTC.c
 *
 *  Created on: May 31, 2021
 *      Author: icepe
 */

#include "NTC.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

#define LEN_NTC_TEMP 							166

#define ADC_MAX_RANGE							4095UL
#define RREF 									10000.0//10k
#define VIN 									3.3

const int Temperature[LEN_NTC_TEMP] = {
-40, -39, -38, -37, -36, -35, -34, -33, -32, -31,
-30, -29, -28, -27, -26, -25, -24, -23, -22, -21,
-20, -19, -18, -17, -16, -15, -14, -13, -12, -11,
-10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,
  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
 10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
 20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
 30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
 40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
 60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
 70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
 90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
120, 121, 122, 123, 124, 125};

const float Resistance[LEN_NTC_TEMP] = {
195.652,  184.9171, 174.8452, 165.391,  156.5125, 148.171,  140.3304,
132.9576, 126.0215, 119.4936, 113.3471, 107.5649, 102.1155,  96.9776,
 92.1315,  87.5588,  83.2424,  79.1663,  75.3157,  71.6768,  68.2367,
 64.9907,  61.919,   59.0113,  56.2579,  53.6496,  51.1779,  48.8349,
 46.6132,  44.5058,  42.5062,  40.5997,  38.7905,  37.0729,  35.4417,
 33.8922,  32.4197,  31.02,    29.689,   28.4231,  27.2186,  26.076,
 24.9877,  23.9509,  22.9629,  22.0211,  21.123,   20.2666,  19.4495,
 18.6698,  17.9255,  17.2139,  16.5344,  15.8856,  15.2658,  14.6735,
 14.1075,  13.5664,  13.0489,  12.554,   12.0805,  11.6281,  11.1947,
 10.7795,  10.3815,  10,        9.6342,   9.2835,   8.947,    8.6242,
  8.3145,   8.0181,   7.7337,   7.4609,   7.1991,   6.9479,   6.7067,
  6.4751,   6.2526,   6.039,    5.8336,   5.6357,   5.4454,   5.2623,
  5.0863,   4.9169,   4.7539,   4.5971,   4.4461,   4.3008,   4.1609,
  4.0262,   3.8964,   3.7714,   3.651,    3.535,    3.4231,   3.3152,
  3.2113,   3.111,    3.0143,   2.9224,   2.8337,   2.7482,   2.6657,
  2.5861,   2.5093,   2.4351,   2.3635,   2.2943,   2.2275,   2.1627,
  2.1001,   2.0396,   1.9811,   1.9245,   1.8698,   1.817,    1.7658,
  1.7164,   1.6685,   1.6224,   1.5777,   1.5345,   1.4927,   1.4521,
  1.4129,   1.3749,   1.3381,   1.3025,   1.268,    1.2343,   1.2016,
  1.17,     1.1393,   1.1096,   1.0807,   1.0528,   1.0256,   0.9993,
  0.9738,   0.9492,   0.9254,   0.9022,   0.8798,   0.858,    0.8368,
  0.8162,   0.7963,   0.7769,   0.758,    0.7397,   0.7219,   0.7046,
  0.6878,   0.6715,   0.6556,   0.6402,   0.6252,   0.6106,   0.5964,
  0.5826,   0.5692,   0.5562,   0.5435,   0.5311};


uint16_t adcStotal = 0, adcStotal2 = 0;
char data_send_buff[256] = { 0 };
uint16_t percent = 0;


extern uint16_t ADC_Sensor_buff[64];

static uint16_t Average(uint16_t *buff, int len) {
	uint32_t temp_value = 0;
	uint16_t return_value = 0;
	temp_value = 0;
	for (int i = 0; i < len; i++) {
		temp_value += buff[i];
	}
	return_value = temp_value / len;
	return return_value;
}

static uint16_t ADC_Sesor_fitter(uint16_t *buff, int len) {
	adcStotal = Average(buff, len);
	if ((adcStotal + 1 == adcStotal2) || (adcStotal - 1 == adcStotal2)
			|| (adcStotal + 2 == adcStotal2) || (adcStotal - 2 == adcStotal2)) {
		adcStotal2 = (adcStotal + adcStotal2 + adcStotal2) / 3;
	} else {
		adcStotal2 = adcStotal;
	}
	return adcStotal2;

}

uint16_t getAdcValue(void) {

	uint16_t ADC_Sensor_value = 0;
	uint16_t ADC_temp_buff[64] = { 0 };

	memset(data_send_buff, 0, sizeof(data_send_buff));
	memcpy(ADC_temp_buff, ADC_Sensor_buff, sizeof(ADC_Sensor_buff));
	ADC_Sensor_value = ADC_Sesor_fitter(ADC_temp_buff, 64);

	return ADC_Sensor_value;
}

float tempNTC(const uint16_t adc_val){

	uint16_t adc_valt = adc_val;
	float Resistance_Nominal = 10;
	float R1 = 10;//kOm
	float R25 = 10;//kOm
	float T;
	float R = R1 * adc_valt/(ADC_MAX_RANGE - adc_valt);
	R = R * Resistance_Nominal / R25;
	if( R <= Resistance[LEN_NTC_TEMP -1])
	{
	  T = Temperature[LEN_NTC_TEMP-1];
	}
	else if( R > Resistance[0])
	{
	  T = Temperature[0];
	}
	else {
	  int i;
	  for (i = 0; ((i < (LEN_NTC_TEMP -1)) && (R <= Resistance[i])); i++);
	  float rDiff = R - Resistance[i - 1];
	  float tRange = Temperature[i] - Temperature[i - 1];
	  float rRange = Resistance[i] - Resistance[i - 1];
	  T = rDiff * tRange / rRange + Temperature[i - 1];
	}

return T;

}

int getBoardTemp(void){
	uint16_t adc_val=0;
	int result=0;
	float temp=0;

	adc_val = adc_updateValue();
	temp=tempNTC(adc_val);
	result=(int)(temp*100);

	if(result<0)
		result=0;

	return result;
}
