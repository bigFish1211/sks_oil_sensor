/*
 * EMA.c
 *
 *  Created on: Aug 31, 2022
 *      Author: icepe
 */

#include "EMA.h"
#include "define.h"

EMA_t SensorEMA5  = {
			.timePreiod = 5,
			.size 		= 0,
			.lastEMA 	= 0
};

EMA_t SensorEMA10  = {
			.timePreiod = 10,
			.size 		= 0,
			.lastEMA 	= 0
};

EMA_t SensorEMA15  = {
			.timePreiod = 15,
			.size 		= 0,
			.lastEMA 	= 0
};

EMA_t SensorEMA30  = {
			.timePreiod = 30,
			.size 		= 0,
			.lastEMA 	= 0
};

EMA_t SensorEMA50  = {
			.timePreiod = 50,
			.size 		= 0,
			.lastEMA 	= 0
};

double calcMA(double previousAverage, unsigned int previousNumDays,
		double newStock) {

	double result = 0;
	result = previousNumDays * previousAverage + newStock;
	return result / (previousNumDays + 1.0);

}

double calcEMA(double previousAverage, int timePeriod, double newStock) {
	double result = 0, mult = 0;
	mult = 2.0 / (timePeriod + 1.0);
	result = (newStock - previousAverage) * mult + previousAverage;
	return result;

}

void addStock(double newStock, EMA_t*EMA){
	EMA->size++;
	if(EMA->size < EMA->timePreiod){
		EMA->lastEMA = calcMA(EMA->lastEMA, EMA->size -1 , newStock);
	}
	else{
		EMA->lastEMA = calcEMA(EMA->lastEMA, EMA->timePreiod, newStock);
	}
}

double getAverage(void) {
	switch (sensorConfig.ema) {
	case EMA5:
		return SensorEMA5.lastEMA;
		break;
	case EMA10:
		return SensorEMA10.lastEMA;
		break;
	case EMA15:
		return SensorEMA15.lastEMA;
		break;
	case EMA30:
		return SensorEMA30.lastEMA;
		break;
	case EMA50:
		return SensorEMA50.lastEMA;
		break;
	default:
		return SensorEMA5.lastEMA;
		break;
	}
}
