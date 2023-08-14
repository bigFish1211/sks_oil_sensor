/**
 ******************************************************************************
 * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c
 * @author  MCD Application Team
 * @version V1.5.0
 * @date    05-December-2014
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "xprintf.h"
#include "string.h"
#include "config.h"
#include "uart.h"
#include "timer.h"
#include "DataUtil.h"
#include "define.h"

/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile extern uint32_t ulSecCount;
volatile extern uint32_t ulTickCount;
volatile extern uint32_t ulMiliCount;
volatile extern unsigned long ulTick100;

uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
uint16_t CaptureNumber = 0;
uint8_t TIM3_isFull = 0;
extern uint32_t Capture;
//extern uint32_t TIM1Freq;
extern uint32_t interrupt_count;
uint32_t pulse_count = 0;
uint32_t last_pulse_count = 0;
uint16_t pulse_per_sec = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {
	ulTickCount++;
	ulMiliCount++;

	if ((ulMiliCount % 100) == 0) { //10 mili seconds
		ulTick100++;
	}

	if (ulMiliCount % 1000 == 0) { //1 seconds
		ulSecCount++;
	}
}

extern char buff_cmd[256];
volatile char buff_cmd_tmp[256] = {0};
int buff_idx = 0;
void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		uint16_t cChar;
		cChar = USART_ReceiveData(USART1);

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		if (buff_idx >= 255 || cChar == '*') {
			buff_idx = 0;
		}

		if(cChar != '\r' && cChar != '\n') {
			buff_cmd_tmp[buff_idx] = cChar;
			buff_idx++;
		}

		//xprintf("len: %d, %c\r\n", buff_idx, cChar);
		//PC_uart_putChar(cChar);

		if (cChar == '#') {
			//copy
			memcpy(buff_cmd, buff_cmd_tmp, buff_idx);
//			xprintf("pc command, len: %d, %s\r\n", buff_idx, buff_cmd);
			buff_idx = 0;
			g_appStats.processPcCommand = 1;
			memset(buff_cmd_tmp, 0, sizeof(buff_cmd_tmp));
		}

		g_appStats.lastReceiveUart = ulSecCount;
	}
}

void TIM15_IRQHandler(void) {
	if (TIM_GetITStatus(TIM15, TIM_IT_CC2) == SET) {
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM15, TIM_IT_CC2);

		if (CaptureNumber == 0) {
			/* Get the Input Capture value */
			IC3ReadValue1 = TIM_GetCapture2(TIM15);
			TIM3_startIT();
			CaptureNumber = 1;
			TIM3_isFull = 0;
		} else if (CaptureNumber == 1) {
			/* Get the Input Capture value */
			CaptureNumber = 0;
			IC3ReadValue2 = TIM_GetCapture2(TIM15);
			if (!TIM3_isFull) {
				Capture = (uint16_t) (IC3ReadValue2 - IC3ReadValue1);
				g_appStats.capture = Capture;
				if(!g_appStats.freqUpdated) {
					g_appStats.tim15Freq = (uint32_t) (TIM15_CLOCK / Capture);
					g_appStats.freqUpdated = 1;
				}
			} else {
				TIM3_isFull = 0;
				if(!g_appStats.freqUpdated) {
					g_appStats.tim15Freq = 0;
					g_appStats.freqUpdated = 1;
				}
			}
		}
	}
}

void TIM3_IRQHandler(void){
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
//		g_appStats.countInterup3++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		TIM3_isFull = 1;
		TIM3_stopIT();
	}
}
/******************************************************************************/
/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
