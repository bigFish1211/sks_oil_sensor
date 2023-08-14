#include "timer.h"
#include <stdint.h>
#include "stm32f0xx_conf.h"

#define OIL_SENOR_IPU_PIN				GPIO_Pin_3
#define OIL_SENOR_IPU_PORT				GPIOA
#define OIL_SENOR_IPU_PIN_SOURCE		GPIO_Pin_3
#define OIL_SENOR_IPU_PIN_AF			GPIO_AF_3
static void TIM3_basicConfiguration();
static void TIM15_captureConfiguration(void);

int timeInit(void){
	TIM3_basicConfiguration();
	TIM15_captureConfiguration();
	return 1;
}

void TIM3_startIT(void){
	TIM_Cmd(TIM3, DISABLE);
	TIM3->CNT =0;
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

}
void TIM3_stopIT(void){
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
}

void TIM3_basicConfiguration(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 2-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable the CC2 Interrupt Request */
	/* TIM enable counter */
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void TIM15_captureConfiguration(void) {
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
	/* GPIOA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* TIM1 channel 1 pin (PA.4) configuration */
	GPIO_InitStructure.GPIO_Pin = OIL_SENOR_IPU_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(OIL_SENOR_IPU_PORT, &GPIO_InitStructure);
	/* Connect TIM pins to AF2 */
	GPIO_PinAFConfig(OIL_SENOR_IPU_PORT, OIL_SENOR_IPU_PIN_SOURCE,
			OIL_SENOR_IPU_PIN_AF);

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 2-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);

	/* TIM1 configuration: Input Capture mode ---------------------
	 The external signal is connected to TIM1 CH1 pin (PA.08)
	 The Rising edge is used as active edge,
	 The TIM1 CCR1 is used to compute the frequency value
	 ------------------------------------------------------------ */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM15, &TIM_ICInitStructure);
	/* Enable the TIM4 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the CC2 Interrupt Request */
	TIM_ITConfig(TIM15, TIM_IT_CC2, ENABLE);
	/* TIM enable counter */
	TIM_Cmd(TIM15, ENABLE);
}
