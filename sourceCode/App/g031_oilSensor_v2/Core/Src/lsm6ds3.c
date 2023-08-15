/*
 * lsm6ds3_Sua.c
 *
 *  Created on: Apr 25, 2023
 *      Author: KhanhNguyen
 */
//////////////////////////////////////////////////////////////
#include "lsm6ds3.h"
#include "string.h"
//#include "register_uart.h"

uint8_t uart_buf[100] = {0};
uint8_t team = 0;
uint8_t teap = 0;
int value = 0;

int Lsm6ds3_int()
{
	team = Lsm6ds3_readRegister(WHO_AM_I_REG);
	if(Lsm6ds3_readRegister(WHO_AM_I_REG) == LSM6DSTR_ADDRESS){
		//set the gyroscope control register to work at 104 Hz, 2000 dps and in bypass mode
		Lsm6ds3_writeRegister(CTRL2_G, 0x4C);
		teap = Lsm6ds3_readRegister(CTRL2_G);

		// Set the Accelerometer control register to work at 104 Hz, 4 g,and in bypass mode and enable ODR/4
		// low pass filter (check figure9 of LSM6DS3's datasheet)
		Lsm6ds3_writeRegister(CTRL1_XL, 0x4A);

		// set gyroscope power mode to high performance and bandwidth to 16 MHz
		Lsm6ds3_writeRegister(CTRL7_G, 0x00);

		// Set the ODR config register to ODR/4
		Lsm6ds3_writeRegister(CTRL8_XL, 0x09);
		return 1;
	}
	else {
		return 0;
	}
}

void Lsm6ds3_end()
{
	Lsm6ds3_writeRegister(CTRL2_G, 0x00);
	Lsm6ds3_writeRegister(CTRL1_XL, 0x00);
}

int Lsm6ds3_readAcceleration(float *x, float* y, float* z)
{
  int16_t data[3]={0};
  long double _data[3]={0};

  if (!Lsm6ds3_readRegisters(OUTX_L_XL, (uint8_t*)data, sizeof(data))) {
    x = 0;
    y = 0;
    z = 0;
    return 0;
  }
  *x = data[0] * 4.0 / 32768.0;
  *y = data[1] * 4.0 / 32768.0;
  *z = data[2] * 4.0 / 32768.0;
  return 1;
}
int acc;
int Lsm6ds3_accelerationAvailable()
{
	acc = Lsm6ds3_readRegister(STATUS_REG);
  if (acc & 0x01) {
    return 1;
  }
  return 0;
}

int Lsm6ds3_readGyroscope(float* x, float* y, float* z)
{
  int16_t data[3]={0};

  if (!Lsm6ds3_readRegisters(OUTX_L_G, (uint8_t*)data, sizeof(data))) {
    x = 0;
    y = 0;
    z = 0;
    return 0;
  }
  *x = data[0] * 2000.0 / 32768.0;
  *y = data[1] * 2000.0 / 32768.0;
  *z = data[2] * 2000.0 / 32768.0;
  return 1;
}

int Lsm6ds3_gyroscopeAvailable()
{
  if (Lsm6ds3_readRegister(STATUS_REG) & 0x02) {
    return 1;
  }
  return 0;
}

float Lsm6ds3_gyroscopeSampleRate()
{
  return 104.0F;
}
int readRegister;
int Lsm6ds3_readRegister(uint8_t address)
{
  uint8_t value;

  if (Lsm6ds3_readRegisters(address, &value, sizeof(value)) != 1) {
	  readRegister= -1;
	  return readRegister;
  }
  return value;
}

int Lsm6ds3_readRegisters(uint8_t address, uint8_t* data, size_t length)
{
	address |= 0x80;
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	SPIx_readWrite(LSM6DS3_SPI, address);
	for(int i=0; i<length; i++){
		*(data+i) = SPIx_readWrite(LSM6DS3_SPI, 0x00);
	}
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	return 1;
}

int Lsm6ds3_writeRegister(uint8_t address, uint8_t value)
{
	//HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	//gpio_write(gpio_port, gpio_pin, gpio_pinState);
	spi2_cs_low();
	SPIx_readWrite(LSM6DS3_SPI, address);
	SPIx_readWrite(LSM6DS3_SPI, value);
	spi2_cs_high();
}

void Lsm6ds3_printRotationAngles() {
   float ax, ay, az;  // accelerometer values
   float gx, gy, gz;  // gyroscope values
   if (Lsm6ds3_accelerationAvailable() && Lsm6ds3_gyroscopeAvailable()
      && Lsm6ds3_readAcceleration(&ax, &ay, &az) && Lsm6ds3_readGyroscope(&gx, &gy, &gz)) {

	      Madgwick_updateIMU(gx, gy, gz, ax, ay, az);
	      roll  = Madgwick_getRoll();
	      pitch = Madgwick_getPitch();
	      yaw   = Madgwick_getYaw();
   }
}






