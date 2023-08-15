/*
 * NTC.h
 *
 *  Created on: May 31, 2021
 *      Author: icepe
 */

#ifndef NTC_H_
#define NTC_H_

#include "NTC.h"
#include "stdio.h"

float tempNTC(const uint16_t adc_val);
int getBoardTemp(void);

#endif /* NTC_H_ */
