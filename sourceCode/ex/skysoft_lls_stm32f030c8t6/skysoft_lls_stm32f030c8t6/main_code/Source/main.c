/**
 ******************************************************************************
 * @file    Project/STM32F0xx_StdPeriph_Templates/main.c
 * @author  MCD Application Team
 * @version V1.5.0
 * @date    05-December-2014
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "stm32f0xx_conf.h"
#include "DataUtil.h"
#include "xprintf.h"
#include "uart.h"
#include "KalmanFilter.h"
#include "command.h"
#include "LSM6DS3.h"
#include "delay.h"
#include "global.h"
#include "config.h"
#include "spi.h"
#include "timer.h"
#include "define.h"
#include "stdlib.h"
#include "internal_temperature.h"
#include "EMA.h"

/** @addtogroup STM32F0xx_StdPeriph_Templates
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define USING_BOOTLOADER		1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USING_BOOTLOADER
#if   (defined ( __CC_ARM ))
__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
__no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#elif defined ( __TASKING__ )
__IO uint32_t VectorTable[48] __at(0x20000000);
#endif

#endif
/* Private function prototypes -----------------------------------------------*/
volatile uint32_t ulSecCount = 0;
volatile uint32_t ulTickCount = 0;
volatile uint32_t ulMiliCount = 0;

volatile uint16_t Capture;

volatile unsigned long ulTick100 = 0; //Tick Count from start to now, 100ms

/* Private functions ---------------------------------------------------------*/
static void sysTick_clock(void);
static void LSM6DS3_init(void);
static void LSM6DS3_get_raw(void);
static void LSM6DS3_get_angle(void);
void static setup(void);

volatile char buff_cmd[256] = {0};

unsigned long lastSentToTracker = 0;
unsigned long lastStoreAverage = 0;

#define  MAX_LEVEL			1023
#define  MAX_NO_AVERAGE 	15
#define  COMPARE_LEVELS 	6
#define  MAX_STORE_LEVEL 	30

STORE_LEVEL avrFreq[MAX_NO_AVERAGE]; //average frequency
STORE_LEVEL min3DFreq[MAX_NO_AVERAGE]; //Save min change
STORE_LEVEL avrFreq3D[MAX_NO_AVERAGE]; //average frequency with 3d
STORE_LEVEL same3DFreq[MAX_NO_AVERAGE]; //Same 3D with previous

STORE_LEVEL storeLevels[MAX_STORE_LEVEL]; //Same 3D with previous

int compare_levels[COMPARE_LEVELS] = {-1};

unsigned char idx = 0;
unsigned char minIdx = 0;
unsigned char sameIdx = 0;
unsigned char idx3D = 0;
uint32_t lastGetTemper = 0;

STORE_LEVEL prePulse;

ERROR_STATE preErrorState;
unsigned long lastBackNormal = 0;

void calc_fuel_level() {
	uint32_t diff = abs(g_config.emptyValue - g_config.fullValue);

	if(g_config.emptyValue > g_config.fullValue && g_config.fullValue && g_config.emptyValue) {
		int fuelLevel = g_appStats.fuelLevel;

		if(g_appStats.emaFrequency == 0) {
			g_appStats.directFuelLevel = 0;
		} else if(g_appStats.emaFrequency <= g_config.fullValue) {
			g_appStats.directFuelLevel = MAX_LEVEL;
		} else if(g_appStats.emaFrequency >= g_config.emptyValue) {
			g_appStats.directFuelLevel = 0;
		} else {
			float nValue = ((((g_config.emptyValue - g_appStats.emaFrequency)*g_config.fullValue))/(float)(diff*g_appStats.emaFrequency))*MAX_LEVEL;
			g_appStats.directFuelLevel = (int)(nValue);
		}

		if(g_appStats.errorState == ERROR_STATE_NORMAL) {
			fuelLevel = g_appStats.directFuelLevel;
		}

		if(preErrorState != ERROR_STATE_NORMAL && g_appStats.errorState == ERROR_STATE_NORMAL) {
			lastBackNormal = ulSecCount;
		}

		preErrorState = g_appStats.errorState;
		float roll = (int)g_appStats.lsm6ds3.roll;
		float pitch = (int)g_appStats.lsm6ds3.pitch;
		float yaw = (int)g_appStats.lsm6ds3.yaw;

		if(g_appStats.errorState == ERROR_STATE_NORMAL && ((!lastBackNormal && ulSecCount >= 3) || ((uint32_t)(ulSecCount - lastBackNormal) >= SKS_TRUNC_ERROR_SEC))) {
			int minSecCount = 0xFFFFFFFF;
			int levelIdx = 0;
			for(int i=0; i<MAX_STORE_LEVEL; i++) {
				STORE_LEVEL *p = &storeLevels[i];
				if(!p->secCount) {
					levelIdx = i;
					break;
				} else if(p->secCount < minSecCount) {
					levelIdx = i;
					minSecCount = p->secCount;
				}
			}

			STORE_LEVEL *p = &storeLevels[levelIdx];
			p->level = fuelLevel;
			p->secCount = ulSecCount;
			p->x = roll;
			p->y = pitch;
			p->z = yaw;
		}

		int maxSecCount = 0;
		int level = -1;
		for(int i=0; i<MAX_STORE_LEVEL; i++) {
			STORE_LEVEL *p = &storeLevels[i];
			if(p->secCount) {
				if(p->secCount < (uint32_t)(ulSecCount - SKS_TRUNC_ERROR_SEC)) {
					if(p->secCount > maxSecCount && p->level) {
						maxSecCount = p->secCount;
						level = p->level;
					}
				} else if(g_appStats.errorState != ERROR_STATE_NORMAL && p->secCount >= (uint32_t)(ulSecCount - SKS_TRUNC_ERROR_SEC)){
					p->secCount = 0;
				}
			}
		}

		if(level != -1) {
			g_appStats.fuelLevel = level;
			g_appStats.lastLevelSecCount = ulSecCount;
		}
	}
}

void output_data() {
	if(ulSecCount < 2) {
		return;
	}

	if(lastGetTemper != ulSecCount) {
		g_appStats.internalTemp = getTemperature();
		lastGetTemper = ulSecCount;
	}

	int roll = (int)g_appStats.lsm6ds3.roll;
	int pitch = (int)g_appStats.lsm6ds3.pitch;
	int yaw = (int)g_appStats.lsm6ds3.yaw;

	if(ulSecCount != lastStoreAverage) {
		calc_fuel_level();

		int fuelLevel = g_appStats.fuelLevel;

		//store min 3D level
		for(int i=0; i<MAX_NO_AVERAGE; i++) {
			if(abs(roll) <= g_config.diffX && abs(pitch) <= g_config.diffY) {
				STORE_LEVEL *pMin = &min3DFreq[i];
				if(pMin->secCount == 0
						|| (ulSecCount - pMin->secCount) > (MAX_NO_AVERAGE*2)
						|| (abs(roll) <= abs(pMin->x) && abs(pitch) <= abs(pMin->y))) {
					pMin->secCount = ulSecCount;
					pMin->frequency = g_appStats.emaFrequency;
					pMin->level = fuelLevel;
					pMin->vibration = g_appStats.vibrInt;
					pMin->x = roll;
					pMin->y = pitch;
					pMin->z = yaw;
					break;
				}
			}
		}
		//end store min 3D level

		//store same 3d level
		if(sameIdx >= MAX_NO_AVERAGE) {
			sameIdx = 0;
		}

		if(abs(roll - prePulse.x) <= 1 && abs(pitch - prePulse.y) <= 1 && abs(roll) <= g_config.diffX && abs(pitch) <= g_config.diffY) {
			same3DFreq[sameIdx].secCount = ulSecCount;
			same3DFreq[sameIdx].frequency = g_appStats.emaFrequency;
			same3DFreq[sameIdx].level = fuelLevel;
			same3DFreq[sameIdx].vibration = g_appStats.vibrInt;
			same3DFreq[sameIdx].x = roll;
			same3DFreq[sameIdx].y = pitch;
			same3DFreq[sameIdx].z = yaw;
			sameIdx++;
		}

		prePulse.x = roll;
		prePulse.y = pitch;
		//end store same 3d level

		//store average level
		if(idx >= MAX_NO_AVERAGE) {
			idx = 0;
		}

		avrFreq[idx].secCount = ulSecCount;
		avrFreq[idx].frequency = g_appStats.emaFrequency;
		avrFreq[idx].level = fuelLevel;
		avrFreq[idx].vibration = g_appStats.vibrInt;
		avrFreq[idx].x = roll;
		avrFreq[idx].y = pitch;
		avrFreq[idx].z = yaw;
		idx++;
		//end store average level

		//store in range 3D
		if(abs(roll) <= g_config.diffX && abs(pitch) <= g_config.diffY) {
			if(idx3D >= MAX_NO_AVERAGE) {
				idx3D = 0;
			}

			avrFreq3D[idx3D].secCount = ulSecCount;
			avrFreq3D[idx3D].frequency = g_appStats.emaFrequency;
			avrFreq3D[idx3D].level = fuelLevel;
			avrFreq3D[idx3D].vibration = g_appStats.vibrInt;
			avrFreq3D[idx3D].x = roll;
			avrFreq3D[idx3D].y = pitch;
			avrFreq3D[idx3D].z = yaw;
			idx3D++;
		}
		//end store in range 3D

		//reset all
		g_appStats.fuelLevel3D = -1; //ok
		g_appStats.fuelLevel3DMin = -1; //ok
		g_appStats.fuelLevel3DSame = -1;
		g_appStats.fuelLevelAvg = -1; //ok
		g_appStats.fuelLevelMinDiff = -1;

		g_appStats.vibrInt = 0;

		lastStoreAverage = ulSecCount;

		unsigned long min3Dx = 0;
		unsigned long min3Dy = 0;
		unsigned long min3DVbr = 0;

		unsigned long same3Dx = 0;
		unsigned long same3Dy = 0;
		unsigned long same3DVbr = 0;
		unsigned long same3DSecCount = 0;


		uint32_t sumAvrFeq = 0;
		int countAvr = 0;

		int sumMinDiff = -1;
		long tmpSecCount = 0;

		for(int i=0; i<MAX_NO_AVERAGE; i++) {
			if(avrFreq[i].secCount) {
				sumAvrFeq += avrFreq[i].level;
				countAvr++;

				//count min diff
				int tmpCountMinDiff = 0;
				int tmpSumMinDiff = 0;
				int sumDiff = 0;
				for(int j=0; j<MAX_NO_AVERAGE; j++) {
					if(avrFreq[j].secCount && i != j) {
						long diff = abs(avrFreq[i].level - avrFreq[j].level);
						tmpSumMinDiff += avrFreq[j].level;
						sumDiff += diff;
						tmpCountMinDiff++;
					}
				}

				//average
				if(tmpCountMinDiff && (sumMinDiff == -1 || (sumDiff < sumMinDiff && avrFreq[i].secCount > tmpSecCount))) {
					tmpSecCount = avrFreq[i].secCount;
					sumMinDiff = sumDiff;
					g_appStats.fuelLevelMinDiff = (tmpSumMinDiff/tmpCountMinDiff);
				}
			}
		}

		int start = 0;
		int startSame = 0;
		long ulSecCountMax = 0;
		uint32_t sumFeq3D = 0;
		int count3D = 0;

		uint32_t minDiffFeq3D = 0;
		int countMinDiff3D = 0;

		for(int i=0; i<MAX_NO_AVERAGE; i++) {
			uint32_t diffTime = (uint32_t)(ulSecCount - avrFreq3D[i].secCount);
			if(avrFreq3D[i].secCount &&  diffTime <= (g_config.timeout3D*60)) {
				if(start == 0) {
					min3Dx = abs(avrFreq3D[i].x);
					min3Dy = abs(avrFreq3D[i].y);
					min3DVbr = avrFreq3D[i].vibration;
					ulSecCountMax = avrFreq3D[i].secCount;
					start++;
				}

				if(abs(avrFreq3D[i].x) <= min3Dx && abs(avrFreq3D[i].y) <= min3Dy && avrFreq3D[i].vibration <= min3DVbr) {
					min3Dx = abs(avrFreq3D[i].x);
					min3Dy = abs(avrFreq3D[i].y);
					min3DVbr = avrFreq3D[i].vibration;
					if(avrFreq3D[i].secCount >= ulSecCountMax) {
						ulSecCountMax = avrFreq3D[i].secCount;
					}
				}

				//sum to calculate average
				sumFeq3D += avrFreq3D[i].level;
				count3D++;

				//check count
				uint32_t tmpMinDiffFeq3D = 0;
				int tmpCount3DMin = 0;
				for(int j=0; j<MAX_NO_AVERAGE; j++) {
					uint32_t diffTime1 = (uint32_t)(ulSecCount - avrFreq3D[j].secCount);
					if(avrFreq3D[j].secCount &&  diffTime1 <= (g_config.timeout3D*60)) {
						int diff = abs(avrFreq3D[i].level - avrFreq3D[j].level);

						if(diff < 10) {
							tmpMinDiffFeq3D += avrFreq3D[j].level;
							tmpCount3DMin++;
						}
					}
				}

				//average
				if(tmpCount3DMin > countMinDiff3D) {
					minDiffFeq3D = tmpMinDiffFeq3D;
					countMinDiff3D = tmpCount3DMin;
				}
			}

			//check same 3d
			uint32_t diffTime2 = (uint32_t)(ulSecCount - same3DFreq[i].secCount);
			if(same3DFreq[i].secCount && diffTime2 <= (g_config.timeout3D*60)) {
				if(startSame == 0) {
					g_appStats.fuelLevel3DSame = same3DFreq[i].level;
					same3Dx = abs(same3DFreq[i].x);
					same3Dy = abs(same3DFreq[i].y);
					same3DVbr = same3DFreq[i].vibration;
					same3DSecCount = same3DFreq[i].secCount;
					startSame++;
				}

				if(abs(same3DFreq[i].x) <= same3Dx && abs(same3DFreq[i].y) <= same3Dy && same3DFreq[i].vibration <= same3DVbr && same3DFreq[i].secCount >= same3DSecCount) {
					g_appStats.fuelLevel3DSame = same3DFreq[i].level;
					same3Dx = abs(same3DFreq[i].x);
					same3Dy = abs(same3DFreq[i].y);
					same3DVbr = same3DFreq[i].vibration;

					if(same3DFreq[i].secCount > same3DSecCount) {
						same3DSecCount = same3DFreq[i].secCount;
					}
				}
			}
		}

		if(countMinDiff3D) {
			g_appStats.fuelLevel3DMin = (minDiffFeq3D/countMinDiff3D);
		}

		if(countAvr > 0) {
			g_appStats.fuelLevelAvg = (sumAvrFeq/countAvr);
		}

		if(count3D) {
			g_appStats.fuelLevel3D = (sumFeq3D/count3D);
		}

		int diff3D = -1;

		if(ulSecCountMax) {
			diff3D = (ulSecCount - ulSecCountMax);
		}

		int step = 0;

		if(g_config.filterMode == AVERAGE_MODE) {
			step = 1;

			if(g_appStats.fuelLevel3DMin != -1) {
				fuelLevel = g_appStats.fuelLevel3DMin;
				step = 11;
			} else if(g_appStats.fuelLevel3D != -1) {
				fuelLevel = g_appStats.fuelLevel3D;
				step = 12;
			} else if(g_appStats.fuelLevelAvg != -1) {
				fuelLevel = g_appStats.fuelLevelAvg;
				step = 13;
			}
		} else if(g_config.filterMode == COMPARE_MODE) {
			step = 2;

			if(g_appStats.fuelLevel3DSame != -1) {
				step = 21;
				fuelLevel = g_appStats.fuelLevel3DSame;
			} else if(g_appStats.fuelLevel3DMin != -1) {
				step = 22;
				fuelLevel = g_appStats.fuelLevel3DMin;
			} else if(g_appStats.fuelLevel3D != -1) {
				step = 23;
				fuelLevel = g_appStats.fuelLevel3D;
			} else if(g_appStats.fuelLevelAvg != -1) {
				step = 24;
				fuelLevel = g_appStats.fuelLevelAvg;
			}

			for(int i=0; i<COMPARE_LEVELS; i++) {
				compare_levels[i] = -1;
			}

			compare_levels[0] = g_appStats.fuelLevel;
			compare_levels[1] = g_appStats.fuelLevelAvg;
			compare_levels[2] = g_appStats.fuelLevel3D;
			compare_levels[3] = g_appStats.fuelLevel3DMin;
			compare_levels[4] = g_appStats.fuelLevel3DSame;
			compare_levels[5] = g_appStats.fuelLevelMinDiff;

			int minDiff = -1;
			long minDiffLevel = -1;
			int countDiff = 0;

			for(int i=0; i<COMPARE_LEVELS; i++) {
				if(compare_levels[i] >= 0) {
					long sumDiff = 0;
					long level = compare_levels[i];

					for(int j=0; j<COMPARE_LEVELS; j++) {
						if(compare_levels[j] >= 0 && i != j) {
							long diff = abs(compare_levels[j] - level);
							sumDiff += diff;
							countDiff++;
						}
					}

					if(countDiff && (minDiff == -1 || (sumDiff/countDiff) < minDiff)) {
						minDiffLevel = level;
						minDiff = (sumDiff/countDiff);
					}
				}
			}

			if(minDiffLevel >= 0) {
				step = 25;
				fuelLevel = minDiffLevel;
			}
		} else if(g_config.filterMode == DIRECT_MODE) {
			//keep value as same current
			step = 3;
		} else if(g_config.filterMode == MIN_MODE) {
			//keep value as same current
			step = 4;
			if(g_appStats.fuelLevelMinDiff != -1) {
				fuelLevel = g_appStats.fuelLevelMinDiff;
				step = 41;
			} else if(g_appStats.fuelLevelAvg != -1) {
				step = 42;
				fuelLevel = g_appStats.fuelLevelAvg;
			}
		} else if(g_config.filterMode == ACCERLATE_MODE_2) {
			step = 5;
			if(g_appStats.fuelLevel3DSame != -1) {
				step = 51;
				fuelLevel = g_appStats.fuelLevel3DSame;
			} else if(g_appStats.fuelLevel3DMin != -1) {
				step = 52;
				fuelLevel = g_appStats.fuelLevel3DMin;
			} else if(g_appStats.fuelLevel3D != -1) {
				step = 53;
				fuelLevel = g_appStats.fuelLevel3D;
			} else if(g_appStats.fuelLevelMinDiff != -1) {
				fuelLevel = g_appStats.fuelLevelMinDiff;
				step = 54;
			} else if(g_appStats.fuelLevelAvg != -1) {
				step = 55;
				fuelLevel = g_appStats.fuelLevelAvg;
			}
		} else if(g_config.filterMode == ACCERLATE_MODE) {
			step = 6;
			if(g_appStats.fuelLevel3DSame != -1) {
				step = 61;
				fuelLevel = g_appStats.fuelLevel3DSame;
			} else if(g_appStats.fuelLevel3DMin != -1) {
				step = 62;
				fuelLevel = g_appStats.fuelLevel3DMin;
			} else if(g_appStats.fuelLevel3D != -1) {
				step = 63;
				fuelLevel = g_appStats.fuelLevel3D;
			} else if(g_appStats.fuelLevelAvg != -1) {
				step = 63;
				fuelLevel = g_appStats.fuelLevelAvg;
			}
		} else { //delay mode
			//direct mode
		}

		if(ulSecCount >= (SKS_TRUNC_ERROR_SEC * 2)) {
			if(g_appStats.debugOn == 1) {
				if(ulSecCount != lastSentToTracker) {
					xprintf("F=%u EF=%u F15=%u N=%d MN=%d XN=%d SN=%d AN=%d CN=%d FU=%u EM=%u x=%d y=%d z=%d idx3D=%d c3D=%d dT=%d step=%d T=%d E=%d\r\n",
							g_appStats.frequency, g_appStats.emaFrequency, g_appStats.tim15Freq,
							fuelLevel, g_appStats.fuelLevel3DMin, g_appStats.fuelLevel3D, g_appStats.fuelLevel3DSame, g_appStats.fuelLevelAvg, g_appStats.directFuelLevel, g_config.fullValue, g_config.emptyValue,
							roll, pitch, yaw, idx3D, count3D, diff3D, step, g_appStats.internalTemp, g_appStats.errorState);
					lastSentToTracker = ulSecCount;
				}
			} else {
				if(g_config.interval && (ulSecCount - lastSentToTracker) >= g_config.interval) {
					if(fuelLevel < 0) {
						fuelLevel = 0;
					}

					if(g_appStats.fuelLevel < 0) {
						g_appStats.fuelLevel = 0;
					}

					if(g_appStats.fuelLevelAvg < 0) {
						g_appStats.fuelLevelAvg = 0;
					}

					if(g_appStats.fuelLevel3DMin < 0) {
						g_appStats.fuelLevel3DMin = 0;
					}

					if(g_appStats.fuelLevel3DSame < 0) {
						g_appStats.fuelLevel3DSame = 0;
					}

					if(g_appStats.fuelLevel3D < 0) {
						g_appStats.fuelLevel3D = 0;
					}

					if(g_appStats.debugOn == 99) {
						xprintf("F=%x EF=%x F15=%x N=%04x.0 XN=%04x.0 AN=%04x.0 CN=%04x.0 MN=%04x.0 SN=%04x.0 x=%d y=%d z=%d idx3D=%d c3D=%d dT=%d tO=%d fM=%d dX=%d dY=%d ST=%d T=%d E=%d\r\n",
								g_appStats.frequency, g_appStats.emaFrequency, g_appStats.tim15Freq,
								fuelLevel, g_appStats.fuelLevel3D, g_appStats.fuelLevelAvg, g_appStats.directFuelLevel, g_appStats.fuelLevel3DMin, g_appStats.fuelLevel3DSame, roll, pitch, yaw,
								idx3D, count3D, diff3D, g_config.timeout3D, g_config.filterMode, g_config.diffX, g_config.diffY, step, g_appStats.internalTemp, g_appStats.errorState);
					} else {
						xprintf("F=%x EF=%x F15=%x N=%04x.0 XN=%04x.0 AN=%04x.0 CN=%04x.0 MN=%04x.0 SN=%04x.0 x=%d y=%d z=%d T=%d E=%d FU=%x EM=%x fM=%d V=%s ST=%d\r\n",
								g_appStats.frequency, g_appStats.emaFrequency, g_appStats.tim15Freq,
								fuelLevel, g_appStats.fuelLevel3D, g_appStats.fuelLevelAvg, g_appStats.directFuelLevel,
								g_appStats.fuelLevel3DMin, g_appStats.fuelLevel3DSame, roll, pitch, yaw,
								g_appStats.internalTemp, g_appStats.errorState, g_config.fullValue, g_config.emptyValue, g_config.filterMode, VERSION_NO, step);
					}

					lastSentToTracker = ulSecCount;
				}
			}
		}
	}
}

long lastReadAccelo;
long lastOut;

uint32_t timer;
short accX, accY, accZ, gyroX, gyroY, gyroZ;
double gyroXrate, gyroYrate, gyroZrate;
double x_rate = 0, y_rate = 0, Z_rate = 0;
double delta;
double kalAngleX, kalAngleY, kalAngleZ;
extern uint16_t calibration;
uint16_t cp = 0, cm = 0;
short sp = 0, sm = 0;
double roll = 0, pitch = 0, yaw = 0;

//extern int buff_idx;
void process_command_from_pc(void) {
    if(buff_cmd[0] == '*') {
		server_on_command(SKS_CMD_SOURCE_COM1, buff_cmd, sizeof(buff_cmd));
		//buff_cmd[0] = 0;
    }

//    if(ulSecCount - g_appStats.lastReceiveUart > 1) {
//    	buff_idx = 0;
//    	buff_cmd[0] = 0;
//    }
}

#define  MAX_FREQ_AVERAGE 5
uint32_t avrTimFreq[MAX_FREQ_AVERAGE] = {0};
int freqIdx = 0;
uint32_t lastFreg = 0;
int cutOff;
void handlerSensorData(void) {
	if(freqIdx < MAX_FREQ_AVERAGE && g_appStats.tim15Freq) {
		avrTimFreq[freqIdx] = g_appStats.tim15Freq;
		freqIdx++;
	} else {
		freqIdx = 0;
	}

	if(freqIdx >= MAX_FREQ_AVERAGE) {
		freqIdx = 0;
	}

	uint32_t sumFreq = 0;
	uint32_t avrFreq = 0;
	int count = 0;
	for(int i=0; i<MAX_FREQ_AVERAGE; i++) {
		if(avrTimFreq[i]) {
			sumFreq += avrTimFreq[i];
			count++;
		}
	}

	if(count > 0) {
		avrFreq = sumFreq/count;
	}

#if SKS_DEBUG_AVG_FREQUENCY
	xprintf("avrFeq: %d, sumFeq: %d, count:%d, currentFrequency: %d, \r\n",avrFreq, sumFreq, count,  g_appStats.tim15Freq);
#endif

	if(avrFreq && g_appStats.tim15Freq) {
		if(!g_config.disableEma) {
			addStock((float)avrFreq, &SensorEMA10);
			avrFreq = SensorEMA10.lastEMA;;
		}
		g_appStats.frequency = avrFreq; //use for set FULL and EMPTY this Frequency
	}

	ERROR_STATE error = ERROR_STATE_NORMAL;

	if(!g_appStats.tim15Freq) {
		error = ERROR_STATE_ZERO;
	} else if(g_config.emptyValue && avrFreq > (g_config.emptyValue * 1.1)) {
		error = ERROR_STATE_OVER_EMPTY;
	} else if(g_config.fullValue && avrFreq < (g_config.fullValue * 0.9)) {
		error = ERROR_STATE_BELOW_FULL;
	} else if(!g_config.fullValue && !g_config.emptyValue) {
		error = ERROR_STATE_NOT_SET_FULL_EMTY;
	}

	g_appStats.errorState = error;

	if(error == ERROR_STATE_NORMAL) {
		if(avrFreq > g_config.emptyValue) {
			avrFreq = g_config.emptyValue;
		} else if (avrFreq < g_config.fullValue){
			avrFreq = g_config.fullValue;
		}

		if(lastFreg) {
			uint32_t diff = abs(avrFreq - lastFreg);
			if(diff > (lastFreg/10) && cutOff < 10) {
				cutOff++;
			} else {
				if(!g_config.disableEma) {
					addStock(avrFreq, &SensorEMA5);
					avrFreq = SensorEMA5.lastEMA;
				}
				g_appStats.emaFrequency = avrFreq;
				cutOff = 0;
			}
		} else {
			if(!g_config.disableEma) {
				addStock(avrFreq, &SensorEMA5);
				avrFreq = SensorEMA5.lastEMA;
			}
			g_appStats.emaFrequency = avrFreq;
		}

		lastFreg = avrFreq;
	}
}

int roll_t, pith_t;
int main(void) {
#ifdef USING_BOOTLOADER
	uint32_t i = 0;
	for (i = 0; i < 48; i++) {
		VectorTable[i] = *(__IO uint32_t*) (APPLICATION_ADDRESS + (i << 2));
	}

	/* Enable the SYSCFG peripheral clock*/
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Remap SRAM at 0x00000000 */
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
#endif

#if SKS_DEBUG_NOW
	g_appStats.debugOn = 1;
#else
	g_appStats.debugOn = 0;
#endif

	setup();

	xprintf("\r\nSystem ready (%s)\r\n", APP_VERSION);
	xprintf("\r\nFULL=%u EMPTY=%u timeout=%d fM=%d interval=%d dX=%d dY=%d DeviceID=%s\r\n\r\n", g_config.fullValue, g_config.emptyValue, g_config.timeout3D, g_config.filterMode, g_config.interval, g_config.diffX, g_config.diffY, g_config.deviceCode);

	while(1){
		if(g_appStats.resetNow) {
			if(ulSecCount - g_appStats.resetNow >= 3) {
				NVIC_SystemReset();
				return 0;
			}
		}

		Kalman_SetFlag();
		LSM6DS3_get_angle();

		if(g_appStats.processPcCommand) {
			process_command_from_pc();
			g_appStats.processPcCommand = 0;
		}

	    cleanWDT();

	    if(g_appStats.freqUpdated) {
	    	handlerSensorData();
	    	g_appStats.freqUpdated = 0;
	    }

	    output_data();

#if 0
		if(last_check_time<ulMiliCount){
			//LSM6DS3_get_raw();
			xprintf("%d		%d		%d\r\n", Capture, g_appStats.frequency, interrupt_count);
			last_check_time = ulMiliCount + 500;
		}
#endif

	}
}

void cleanWDT(void){
	IWDG_SetWindowValue(1000);
}

/*In this case the refresh window is:
  (2000-1000) / (LSI/16) = ~400 ms < refresh window < 2000 / (LSI/16) = ~800 ms
  */
void IWDG_init(void){
	IWDG_Enable();
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	/* Set IWDG Prescaler value to 16 */
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	/* Set IWDG Reload value to 2000 */
	IWDG_SetReload(2000);
	/* Wait until RVU flag is reset to be sure that the reload value
	update operation is completed */
	while(IWDG_GetFlagStatus(IWDG_FLAG_RVU) != RESET);
	/* Set the IWDG window value to 1000 */
	IWDG_SetWindowValue(1000);
}

void static setup(void){
	SystemInit();
	sysTick_clock();
	PC_uartInit(PC_BAUDRATE);

	//init flash
	FlashInit();
	load_config();

	xdev_out(PC_uart_putChar);
	timeInit();
	internalTempInit();

	LSM6DS3_init();
	Start_Kalman_Algorithm(10, 1000000.0f);

	IWDG_init();
////	g_appStats.debug =1;

//	xprintf("Init done!\r\n");
}

static void sysTick_clock(void){

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	if (SysTick_Config(SystemCoreClock / 1000))
		while (1);
}

static void LSM6DS3_init(void){
	uint8_t test =0;
	LSM6DS3_Initialize(LSM6DS3_SPI,LSM6DS3_DEFAULT_TIMEOUT);
	delay_ms(200);

	if(LSM6DS3_Detect(10) == LSM6DS3_OK)
	{
		//xprintf("LSM6DS3 found\r\n");
		if(LSM6DS3_XL_Start(XL_1666Hz, XL_RANGE_16G, XL_FILTER_400Hz)==LSM6DS3_FAIL){
			//xprintf("LSM6DS3_XL_Start fail\r\n");
		}
		else{
			//xprintf("LSM6DS3_XL_Start ok\r\n");
		}
		delay_ms(10);

		if(LSM6DS3_GS_Start(GS_1666Hz, GS_RANGE_2000dps)!=LSM6DS3_OK){
			//xprintf("LSM6DS3_GS_Start fail\r\n");
		}
		else{
			//xprintf("LSM6DS3_GS_Start ok\r\n");
		}
		delay_ms(10);

  }
  else
  {
//		xprintf("LSM6DS3 not found\r\n");
	  /* Handle the error */
  }
}

static void LSM6DS3_get_raw(void){
	if(LSM6DS3_IMU_GetMeasurements()!=LSM6DS3_OK){
		xprintf("LSM6DS3_IMU_GetMeasurements fail \r\n");
	}
	xprintf("XL			%d-%d-%d\r\n",LSM6DS3_GetXL_X_Int16(),LSM6DS3_GetXL_Y_Int16(),LSM6DS3_GetXL_Z_Int16());
	xprintf("GS			%d-%d-%d\r\n",LSM6DS3_GetGS_X_Int16(),LSM6DS3_GetGS_Y_Int16(),LSM6DS3_GetGS_Z_Int16());
}


static void LSM6DS3_get_angle(void){
	char buf[255]= {0};

	if(Kalman_GetFlag()) {
		//xprintf("%s-%d\r\n",__func__,__LINE__);
		if(LSM6DS3_IMU_GetMeasurements()==LSM6DS3_OK){
			Execute_Kalman_Filter();
		}

		if (g_appStats.last_send_data < ulMiliCount) {
			memset(buf, 0, sizeof(buf));
			g_appStats.lsm6ds3.roll = Kalman_GetRoll();
			g_appStats.lsm6ds3.pitch = Kalman_GetPitch();
			g_appStats.lsm6ds3.yaw = Kalman_Getyaw();
			g_appStats.lsm6ds3.T = LSM6DS3_Temperature_Int16();

#if 0
			xprintf("Roll = %d & Pitch = %d & yaw = %d & Temperature = %d\r\n",
					(int) g_appStats.lsm6ds3.roll,
					(int) g_appStats.lsm6ds3.pitch,
					(int) g_appStats.lsm6ds3.yaw, (int) g_appStats.lsm6ds3.T);
#endif

			g_appStats.last_send_data = ulMiliCount + 500;
		}

		Kalman_ClearFlag();
	}
}
/**
 * @}
 */

/************************ (C) COPYRIGHT Skysoft Technologies *****END OF FILE****/
