/*
 * g031_system.h
 *
 *  Created on: Jul 27, 2022
 *      Author: icepe
 */

#ifndef INC_SYSTEM_G031_H_
#define INC_SYSTEM_G031_H_

#include <stdint.h>

#define systemClock						64000000UL//64M
#define SysTick_CLKSource_HCLK         ((uint32_t)0x00000004)

void systemInit(void);
void RCC_DeInit(void);

#endif /* INC_SYSTEM_G031_H_ */
