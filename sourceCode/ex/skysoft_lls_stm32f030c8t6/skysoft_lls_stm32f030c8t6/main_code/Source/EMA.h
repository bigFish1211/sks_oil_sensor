/*
 * EMA.h
 *
 *  Created on: Sep 7, 2022
 *      Author: icepe
 */

#ifndef INC_EMA_H_
#define INC_EMA_H_
#include <stdint.h>

typedef struct {
	uint16_t timePreiod;
	uint32_t size;
	double lastEMA;
} EMA_t;

extern EMA_t SensorEMA5;
extern EMA_t SensorEMA10;
void addStock(double newStock, EMA_t*EMA);
double getAverage(void);
#endif /* INC_EMA_H_ */
