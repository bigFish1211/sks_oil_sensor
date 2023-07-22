/*
 * MadgwickAHRS.h
 *
 *  Created on: Apr 24, 2023
 *      Author: KhanhNguyen
 */

#ifndef INC_MADGWICKAHRS_H_
#define INC_MADGWICKAHRS_H_

#include <math.h>
#include "main.h"

//--------------------------------------------------------------------------------------------
// Variable declaration
    #define SAMPLE_RATE 10  // in Hz

    extern float beta;				// algorithm gain
    extern float q0;
    extern float q1;
    extern float q2;
    extern float q3;	// quaternion of sensor frame relative to auxiliary frame
    extern float invSampleFreq;
    extern float roll;
    extern float pitch;
    extern float yaw;
    extern char anglesComputed;
    void Madgwick_computeAngles();
    static float Madgwick_invSqrt(float x);

//-------------------------------------------------------------------------------------------
// Function declarations
    void Madgwick_Madgwick(void);
    void Madgwick_init(float sampleFrequency);
    void Madgwick_update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    void Madgwick_updateIMU(float gx, float gy, float gz, float ax, float ay, float az);

    float Madgwick_getRoll();
    float Madgwick_getPitch();
    float Madgwick_getYaw();
    float Madgwick_getRollRadians();
    float Madgwick_getPitchRadians();
    float Madgwick_getYawRadians();

#endif /* INC_MADGWICKAHRS_H_ */
