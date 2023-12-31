/*
  ******************************************************************************
  * @file    KalmanFilter.c
  * @author  Savvas Kokkinidis - sabbaskok@hotmail.com
  * @version V1.0
  * @date    31-July-2020
  * @brief   This file contains the functions related to the
  *          the Kalman Filter
  ******************************************************************************
 */

#include "KalmanFilter.h"
#include "LSM6DS3.h"
#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "delay.h"
#include "global.h"
#include "config.h"
#include "xprintf.h"

#define RAD_TO_DEG  	57.295779f

#define RESTRICT_PITCH

static float KalmanX_getAngle(float newAngle, float newRate, float dt);
static float KalmanY_getAngle(float newAngle, float newRate, float dt);

Kalman_struct KalmanX;
Kalman_struct KalmanY;
volatile double gyroZ_offset = 14;

double kalAngleZ=0;

__IO static uint32_t Kalman_Timer, ExecuteTicks , ExecuteMax;
__IO static uint8_t KalmanFlag;
static float Calculated_Pitch, Calculated_Roll, Calculated_yaw,Tick_frequency;

extern void IWDT_clear(void);

void Start_Kalman_Algorithm(float executeMaxTicks, float timebase)
{
    double accX, accY, accZ; 
  
    /* We will set the variables like so, these can also be tuned by the user */
    KalmanX.Q_angle      = 0.001f;
    KalmanX.Q_bias       = 0.003f;
    KalmanX.R_measure    = 0.03f;
    KalmanX.angle        = 0.0f;     
    KalmanX.bias         = 0.0f;
    KalmanX.P[0][0]      = 0.0f; 
    KalmanX.P[0][1]      = 0.0f;
    KalmanX.P[1][0]      = 0.0f;
    KalmanX.P[1][1]      = 0.0f;
    
    KalmanY.Q_angle      = 0.001f;
    KalmanY.Q_bias       = 0.003f;
    KalmanY.R_measure    = 0.03f;
    KalmanY.angle        = 0.0f;     
    KalmanY.bias         = 0.0f;
    KalmanY.P[0][0]      = 0.0f; 
    KalmanY.P[0][1]      = 0.0f;
    KalmanY.P[1][0]      = 0.0f;
    KalmanY.P[1][1]      = 0.0f;
    
    
    LSM6DS3_IMU_GetMeasurements();
    accX =   LSM6DS3_GetXL_X_Int16();
    accY =   LSM6DS3_GetXL_Y_Int16();
    accZ =   LSM6DS3_GetXL_Z_Int16();
    
    #ifdef RESTRICT_PITCH
        double roll  = atan2(accY, accZ) * RAD_TO_DEG;
        double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
    #else 
        double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
        double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
    #endif
    
   KalmanX.angle    = roll;
   KalmanY.angle 	= pitch;
   Tick_frequency	= timebase;
   ExecuteMax	    = executeMaxTicks;
   Kalman_Timer  	= 0;
}
   
	double accX, accY, accZ;
	double gyroX,gyroY,gyroZ;

void Execute_Kalman_Filter(void) 
{
  double timing_kalman;
  double kalAngleX=0;
  double kalAngleY=0;
	//double accX, accY, accZ;
	//double gyroX,gyroY,gyroZ;
  static uint32_t current_time,last_time = 0;

  current_time = Kalman_GetTick();
  timing_kalman = (double) (((uint32_t)(current_time - last_time)) / Tick_frequency);
  last_time = current_time;

  accX  = LSM6DS3_GetXL_X_Int16();
  accY  = LSM6DS3_GetXL_Y_Int16();
  accZ  = LSM6DS3_GetXL_Z_Int16();
  gyroX = LSM6DS3_GetGS_X_Int16();
  gyroY = LSM6DS3_GetGS_Y_Int16();
  gyroZ = LSM6DS3_GetGS_Z_Int16();

  #ifdef RESTRICT_PITCH
    double roll  = atan2(accY, accZ) * RAD_TO_DEG;
    double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
  #else // Eq. 28 and 29
    double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  #endif
       
	double gyroXrate = gyroX * 0.007f; /* Convert to deg/s */
	double gyroYrate = gyroY * 0.007f; /* Convert to deg/s */
	double gyroZrate = (gyroZ-gyroZ_offset) * 0.007f; /* Convert to deg/s */
	kalAngleZ = kalAngleZ+gyroZrate*timing_kalman;

	if(kalAngleZ>360){
		kalAngleZ-=360;
	}
	else if(kalAngleZ<-360){
		kalAngleZ+=360;
	}

  #ifdef RESTRICT_PITCH
  /* This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees */
  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) 
  {
	KalmanX.angle = roll;
    kalAngleX = roll;
  } 
  else
  {
    kalAngleX = KalmanX_getAngle(roll, gyroXrate, timing_kalman); /* Calculate the angle using a Kalman filter */
  } 

  if (abs(kalAngleX) > 90)
  {
    gyroYrate = - gyroYrate; /* Invert rate, so it fits the restricted accelerometer reading */
  }
  kalAngleY = KalmanY_getAngle(pitch, gyroYrate, timing_kalman);
  #else
    /* This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees */
    if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) 
    {
      KalmanY.angle = pitch;
      kalAngleY = pitch;
    } 
    else
    {
      kalAngleY = KalmanY_getAngle(pitch, gyroYrate, timing_kalman); /* Calculate the angle using a Kalman filter */
    }

    if (abs(kalAngleY) > 90)
    {
      gyroXrate = -gyroXrate; /* Invert rate, so it fits the restriced accelerometer reading */
    }
    kalAngleX = KalmanX_getAngle(roll, gyroXrate, timing_kalman);
  #endif
   Calculated_Roll  =  (float) (kalAngleX);
   Calculated_Pitch  = (float) (kalAngleY);
   Calculated_yaw  = (float) (kalAngleZ);
}

float Kalman_GetPitch(void) {
  return Calculated_Pitch;
}

float Kalman_GetRoll(void) {
  return Calculated_Roll;
}

float Kalman_Getyaw(void) {
#if 1
  return Calculated_yaw;
#else
  static uint32_t current_time,last_time = 0;
  double timing_kalman;

  current_time = Kalman_GetTick();
  timing_kalman = (double) (((uint32_t)(current_time - last_time)) / Tick_frequency);
  last_time = current_time;

  gyroZ = LSM6DS3_GetGS_Z_Int16();

  double gyroZrate = (gyroZ-gyroZ_offset) * 0.007f; /* Convert to deg/s */
  	kalAngleZ = kalAngleZ+gyroZrate*timing_kalman;
  	return kalAngleZ;
#endif
}

void Kalman_Getyaw_execute(void) {
#if 0
  return Calculated_yaw;
#else
  static uint32_t current_time,last_time = 0;
  double timing_kalman;

  current_time = Kalman_GetTick();
  timing_kalman = (double) (((uint32_t)(current_time - last_time)) / Tick_frequency);
  last_time = current_time;

  gyroZ = LSM6DS3_GetGS_Z_Int16();

  double gyroZrate = (gyroZ-gyroZ_offset) * 0.007f; /* Convert to deg/s */
  	kalAngleZ = kalAngleZ+gyroZrate*timing_kalman;
    Calculated_yaw  = (float) (kalAngleZ);
#endif
}


void Kalman_IncTick(void) {
	Kalman_Timer++;
	ExecuteTicks++;
	if(ExecuteTicks >= ExecuteMax) {
		Kalman_SetFlag();
		ExecuteTicks = 0;
	}
}

uint32_t Kalman_GetTick(void)
{
	//return Kalman_Timer;
	return getMicros();
}

void Kalman_SetFlag(void) {
	KalmanFlag = 1;
}

uint8_t Kalman_GetFlag(void) {
	return KalmanFlag;
}

void Kalman_ClearFlag(void) {
	KalmanFlag = 0;
}

static float KalmanX_getAngle(float newAngle, float newRate, float dt) {
    KalmanX.rate = newRate - KalmanX.bias;
    KalmanX.angle += dt * KalmanX.rate;

    KalmanX.P[0][0] += dt * (dt*KalmanX.P[1][1] - KalmanX.P[0][1] - KalmanX.P[1][0] + KalmanX.Q_angle);
    KalmanX.P[0][1] -= dt * KalmanX.P[1][1];
    KalmanX.P[1][0] -= dt * KalmanX.P[1][1];
    KalmanX.P[1][1] += KalmanX.Q_bias * dt;
    
    float S = KalmanX.P[0][0] + KalmanX.R_measure;
    
    float K[2];
    K[0] = KalmanX.P[0][0] / S;
    K[1] = KalmanX.P[1][0] / S;

    float y = newAngle - KalmanX.angle;
    KalmanX.angle += K[0] * y;
    KalmanX.bias += K[1] * y;

    float P00_temp = KalmanX.P[0][0];
    float P01_temp = KalmanX.P[0][1];

    KalmanX.P[0][0] -= K[0] * P00_temp;
    KalmanX.P[0][1] -= K[0] * P01_temp;
    KalmanX.P[1][0] -= K[1] * P00_temp;
    KalmanX.P[1][1] -= K[1] * P01_temp;

    return KalmanX.angle;
}

static float KalmanY_getAngle(float newAngle, float newRate, float dt) {
    KalmanY.rate = newRate - KalmanY.bias;
    KalmanY.angle += dt * KalmanY.rate;

    KalmanY.P[0][0] += dt * (dt*KalmanY.P[1][1] - KalmanY.P[0][1] - KalmanY.P[1][0] + KalmanY.Q_angle);
    KalmanY.P[0][1] -= dt * KalmanY.P[1][1];
    KalmanY.P[1][0] -= dt * KalmanY.P[1][1];
    KalmanY.P[1][1] += KalmanY.Q_bias * dt;
    
    float S = KalmanY.P[0][0] + KalmanY.R_measure;
    
    float K[2];
    K[0] = KalmanY.P[0][0] / S;
    K[1] = KalmanY.P[1][0] / S;


    float y = newAngle - KalmanY.angle;

    KalmanY.angle += K[0] * y;
    KalmanY.bias += K[1] * y;

    float P00_temp = KalmanY.P[0][0];
    float P01_temp = KalmanY.P[0][1];

    KalmanY.P[0][0] -= K[0] * P00_temp;
    KalmanY.P[0][1] -= K[0] * P01_temp;
    KalmanY.P[1][0] -= K[1] * P00_temp;
    KalmanY.P[1][1] -= K[1] * P01_temp;

    return KalmanY.angle;
}

void LSM6DS3_calibaration(uint16_t time){
#if 0
	int32_t gyroX_total=0,gyroY_total=0,gytoZ_total=0;
	xprintf("LSM6DS3 calibration start\r\n");
	for(int i = 0;i<time;i++){
	
		gyroX_total += LSM6DS3_GetGS_X_Int16();
		gyroY_total += LSM6DS3_GetGS_Y_Int16();
		gytoZ_total += LSM6DS3_GetGS_Z_Int16();
	
		delay_ms(100);
		xprintf(".");
		IWDT_clear();
	}
	
	g_config.lsm6ds3_offset.gyroX_offset = gyroX_total/time;
	g_config.lsm6ds3_offset.gyroY_offset = gyroY_total/time;
	g_config.lsm6ds3_offset.gyroZ_offset = gytoZ_total/time;
	
	g_config.lsm6ds3_offset.accX_offset=0;
	g_config.lsm6ds3_offset.accY_offset=0;
	g_config.lsm6ds3_offset.accZ_offset=0;
		
	
	g_appStats.writeConfigNow = true;
	
	xprintf("\r\n");
	xprintf("gyroX_offset=%d\r\n",g_config.lsm6ds3_offset.gyroX_offset);
	xprintf("gyroY_offset=%d\r\n",g_config.lsm6ds3_offset.gyroY_offset);
	xprintf("gyroZ_offset=%d\r\n",g_config.lsm6ds3_offset.gyroZ_offset);
	
	xprintf("LSM6DS3 calibration done \r\n");
	
#endif
}


