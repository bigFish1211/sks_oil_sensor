
#include "gpio.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

#define LED_RED_PIN				GPIO_Pin_7
#define LED_RED_PORT			GPIOB
#define LED_RED_RCC				RCC_AHBPeriph_GPIOA


#define BUZZ_PIN				GPIO_Pin_15
#define BUZZ_PORT				GPIOB
#define BUZZ_RCC				RCC_AHBPeriph_GPIOA

#define CR95HF_EN_PIN			GPIO_Pin_12
#define CR95HF_EN_PORT			GPIOB
#define CR95HF_EN_RCC			RCC_AHBPeriph_GPIOA

#define LED_RED_ON()			GPIO_ResetBits(LED_RED_PORT,LED_RED_PIN)
#define LED_RED_OFF()			GPIO_SetBits(LED_RED_PORT,LED_RED_PIN)
#define LED_RED_TOOGLE() 		GPIO_WriteBit(LED_RED_PORT,LED_RED_PIN,!GPIO_ReadInputDataBit(LED_RED_PORT,LED_RED_PIN))

#define BUZZ_ON()				GPIO_ResetBits(LED_RED_PORT,LED_RED_PIN)
#define BUZZ_OFF()				GPIO_SetBits(LED_RED_PORT,LED_RED_PIN)

#define CR95HF_EN()				GPIO_SetBits(CR95HF_EN_PORT,CR95HF_EN_PIN)
#define CR95HF_DIS()			GPIO_ResetBits(CR95HF_EN_PORT,CR95HF_EN_PIN)

void static ledRedInit(void);
void static buzzInit(void);
void static Cr65hfEnInit(void);

void gpio_conf(void){
	ledRedInit();
	buzzInit();
	Cr65hfEnInit();
}

void BuzzOff(void) {
	BUZZ_OFF();
}

void BuzzOn(void) {
	BUZZ_ON();
}

void RedLedOn(void) {
	LED_RED_ON();
}

void RedLedOff(void) {
	LED_RED_OFF();
}

void cr95hfEn(void) {
	CR95HF_EN();
}

void cr95hfDis(void) {
	CR95HF_DIS();
}

void static ledRedInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(LED_RED_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);

}
void static buzzInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(BUZZ_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUZZ_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(BUZZ_PORT, &GPIO_InitStructure);

}
void static Cr65hfEnInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(CR95HF_EN_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = CR95HF_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CR95HF_EN_PORT, &GPIO_InitStructure);

}
