/*
 * internal_temperature.h
 *
 *  Created on: Sep 19, 2022
 *      Author: icepe
 */

#ifndef INTERNAL_TEMPERATURE_H_
#define INTERNAL_TEMPERATURE_H_
#include <stdint.h>

void internalTempInit(void);
uint32_t getTemperature(void);
#endif /* INTERNAL_TEMPERATURE_H_ */
