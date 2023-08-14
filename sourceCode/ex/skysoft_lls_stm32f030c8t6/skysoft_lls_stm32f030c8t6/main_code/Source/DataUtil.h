#include <string.h>
#include <time.h>
#include <global.h>
#include <stdint.h>

#ifndef __SKS_DATA_UTIL
#define __SKS_DATA_UTIL

#define BUFFER_32 32
#define BUFFER_MSG 256

#define DEVICE_CODE_LEN 10

#define SKS_BUFFER_LEN 256

typedef enum _FILTER_MODE {
	DELAY_MODE,
	AVERAGE_MODE,
	COMPARE_MODE,
	MIN_MODE,
	DIRECT_MODE,
	ACCERLATE_MODE_2,
	ACCERLATE_MODE
} FILTER_MODE;

typedef struct _DEVICE_CONFIG
{
	char sign;
	unsigned char configState;
	unsigned char deviceCode[DEVICE_CODE_LEN];
	unsigned int fullValue;
	unsigned int emptyValue;
	unsigned short timeout3D; //lock position in minutes before timeout
	unsigned char outputMode; //=0, Disable out, 1=TEXT, 2=Binary
	unsigned char filterMode; //interval to ouput
	unsigned char diffX; //interval to output
	unsigned char diffY; //interval to output
	unsigned char interval;
	unsigned char disableEma;
}
DEVICE_CONFIG;

//typedef struct _FREQUENCE_PULSE
//{
//	uint32_t secCount;
//	uint32_t frequency;
//	uint32_t vibration;
//	int x;
//	int y;
//	int z;
//} FREQUENCE_PULSE;

typedef struct _STORE_LEVEL
{
	uint32_t secCount;
	uint32_t frequency;
	uint32_t vibration;
	int x;
	int y;
	int z;
	int level;
} STORE_LEVEL;

typedef struct _ACC_GYRO{
	float roll;
	float pitch;
	float yaw;
	float T;
	int16_t accX_offset;
	int16_t accY_offset;
	int16_t accZ_offset;

	int16_t gyroX_offset;
	int16_t gyroY_offset;
	int16_t gyroZ_offset;

} ACC_GYRO;

typedef struct _SKS_APP_STATS
{
	ACC_GYRO lsm6ds3;
	uint32_t tim15Freq;
	uint32_t frequency;
	uint32_t emaFrequency;
	uint32_t fuelLevel;
	uint32_t directFuelLevel;
	uint32_t lastLevelSecCount;
	int fuelLevelAvg;
	int fuelLevel3D;
	int fuelLevel3DMin;
	int fuelLevel3DSame;
	int fuelLevelMinDiff;
	uint32_t vibrInt;
	unsigned char temperature;
	unsigned char debugOn;
	uint32_t resetNow;
	uint32_t last_send_data;
	uint32_t lastReceiveUart;
	char writeConfig;
	uint32_t internalTemp;
	char freqUpdated;
	ERROR_STATE errorState;
	char processPcCommand;
	uint16_t capture;
}

SKS_APP_STATS;

DEVICE_CONFIG g_config;
SKS_APP_STATS g_appStats;
#endif
