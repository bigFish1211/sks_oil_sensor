/*
 * system_g031.c
 *
 *  Created on: Jun 15, 2022
 *      Author: icepe
 */

#include "system_g031.h"
#include "stm32g0xx.h"

#define HSI_VALUE    (16000000UL)            /*!< Value of the Internal oscillator in Hz*/

uint32_t SystemCoreClock = 64000000UL;

#if !defined  (HSI_STARTUP_TIMEOUT)
#define HSI_STARTUP_TIMEOUT   ((uint16_t)0x5000) /*!< Time out for HSI start up */
#endif /* HSI_STARTUP_TIMEOUT */

static void SetSysClock(void);
static void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);
static void systickConfig(void);

void systemInit(void) {
	SetSysClock();
	systickConfig();
}

static void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource) {

	if (SysTick_CLKSource == SysTick_CLKSource_HCLK) {
		SysTick->CTRL |= SysTick_CLKSource_HCLK;
	}
}

static void systickConfig(void) {
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_Config(systemClock / 1000);
}

static void SetSysClock(void) {
	/* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
	/* Enable HSE */
	RCC->CR |= ((uint32_t) RCC_CR_HSEON);
	while ((RCC->CR & RCC_CR_HSERDY) == 0)
		;

	/* Enable Prefetch Buffer and set Flash Latency */
	FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_2;
	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	// APB PR
	RCC->CFGR &= (uint32_t) (~RCC_CFGR_HPRE);
	// AHB PR
	RCC->CFGR &= (uint32_t) (~RCC_CFGR_PPRE);

	//PLL INPUT CLOCK SOURCE HSE
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;

	/* PLL configuration = HSE = 64 MHz */
	//PLLM = 1
	//PLLN = 16
	//PLLR = 2
	RCC->PLLCFGR &= (uint32_t) ((uint32_t) ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN
			| RCC_PLLCFGR_PLLR));

	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLN_4)
			| (RCC_PLLCFGR_PLLR_0 | RCC_PLLCFGR_PLLREN) | (RCC_PLLCFGR_PLLQEN)
			| (RCC_PLLCFGR_PLLSRC_HSE);

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait till PLL is ready */
	while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
	}

	/* Select PLL as system clock source */
	RCC->CFGR &= (uint32_t) ((uint32_t) ~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t) RCC_CFGR_SW_1;

	/* Wait till PLL is used as system clock source */
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1)
		;
}

void RCC_DeInit(void){
	  /* Set HSION bit to the reset value */
		RCC->CR |= RCC_CR_HSION;
	  /* Wait till HSI is ready */
		while((RCC->CR& RCC_CR_HSIRDY)==0);
	  /* Set HSITRIM[6:0] bits to the reset value */
	  RCC->ICSCR = RCC_ICSCR_HSITRIM_6;
	  /* Reset CFGR register (HSI is selected as system clock source) */
	  RCC->CFGR = 0x00000000u;
	  /* Wait till HSI is ready */
	  while((RCC->CFGR & RCC_CFGR_SWS) != 0U)
	  /* Clear CR register in 2 steps: first to clear HSEON in case bypass was enabled */
	  RCC->CR = RCC_CR_HSION;
	  /* Then again to HSEBYP in case bypass was enabled */
	  RCC->CR = RCC_CR_HSION;
	  /* Wait till PLL is ready */
	  while ((RCC->CR & RCC_CR_PLLRDY) != 0U)
	  /* once PLL is OFF, reset PLLCFGR register to default value */
	  RCC->PLLCFGR = RCC_PLLCFGR_PLLN_4;
	  /* Disable all interrupts */
	  RCC->CIER = 0x00000000u;
	  /* Clear all flags */
	  RCC->CICR = 0xFFFFFFFFu;
	  /* Update the SystemCoreClock global variable */
	  SystemCoreClock = HSI_VALUE;
}
