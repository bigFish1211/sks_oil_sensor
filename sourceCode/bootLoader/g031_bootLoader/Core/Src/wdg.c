/*
 * wdg.c
 *
 *  Created on: Mar 5, 2022
 *      Author: icepe
 */

#include "wdg.h"


void IWDG_Init(void)
{


}

void IWDG_start(void){
	 IWDG->KR = 0xCCCC;
}

void IWDG_clear(void){
	IWDG->KR = 0xAAAA;
}
