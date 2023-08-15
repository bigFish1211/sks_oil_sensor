/*
 * lsm6ds3_Sua.h
 *
 *  Created on: Apr 25, 2023
 *      Author: KhanhNguyen
 */
////////////////////////////////////////////////////
#ifndef INC_LSM6DS3_H_
#define INC_LSM6DS3_H_

#include "main.h"
//#include <stm32g031xx.h>
#include <stdint.h>
//#include "spi.h"
#include "drv_spi.h"
#include "MadgwickAHRS.h"

//extern SPI_HandleTypeDef hspi2;

#define LSM6DSTR_ADDRESS   0x6B
#define WHO_AM_I_REG       0X0F
#define CTRL1_XL           0X10
#define CTRL2_G            0X11

#define STATUS_REG         0X1E

#define CTRL6_C            0X15
#define CTRL7_G            0X16
#define CTRL8_XL           0X17

#define OUTX_L_G           0X22
#define OUTX_H_G           0X23
#define OUTY_L_G           0X24
#define OUTY_H_G           0X25
#define OUTZ_L_G           0X26
#define OUTZ_H_G           0X27

#define OUTX_L_XL          0X28
#define OUTX_H_XL          0X29
#define OUTY_L_XL          0X2A
#define OUTY_H_XL          0X2B
#define OUTZ_L_XL          0X2C
#define OUTZ_H_XL 0X2D

#define 	LSM6DS3_SPI		SPI2

int Lsm6ds3_int();
void Lsm6ds3_end();

// Accelerometer
int Lsm6ds3_readAcceleration(float* x, float* y, float* z); // Results are in g (earth gravity).
int Lsm6ds3_accelerationAvailable(); // Check for available data from accelerometer

// Gyroscope
int Lsm6ds3_readGyroscope(float* x, float* y, float* z); // Results are in degrees/second.
float Lsm6ds3_gyroscopeSampleRate(); // Sampling rate of the sensor.
int Lsm6ds3_gyroscopeAvailable(); // Check for available data from gyroscope

int Lsm6ds3_readRegister(uint8_t address);
int Lsm6ds3_readRegisters(uint8_t address, uint8_t* data, size_t length);
int Lsm6ds3_writeRegister(uint8_t address, uint8_t value);
//void printRotationAngles();
void Lsm6ds3_printRotationAngles();



//extern LSM6DS3Class IMU_LSM6DS3;
//#undef IMU
// #define IMU IMU_LSM6DS3

#endif /* INC_LSM6DS3_H_ */
