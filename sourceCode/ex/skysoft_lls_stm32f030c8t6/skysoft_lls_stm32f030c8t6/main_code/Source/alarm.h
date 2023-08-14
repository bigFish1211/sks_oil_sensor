/*
 * alarm.h
 *
 *  Created on: Mar 31, 2013
 *      Author: Administrator
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "config.h"
#include "stm32f0xx.h"

#if (MATRIX_VERSION == 0x01)
	#define BUZZ_PIN GPIO_Pin_1
	#define BUZZ_PORT GPIOA

	#define RED_LED_PIN GPIO_Pin_2
	#define RED_LED_PORT GPIOA

	#define GREEN_LED_PIN GPIO_Pin_8
	#define GREEN_LED_PORT GPIOB
#elif (MATRIX_VERSION == 0x02)
	#define BUZZ_PIN GPIO_Pin_0
	#define BUZZ_PORT GPIOB

	#define RED_LED_PIN GPIO_Pin_14
	#define RED_LED_PORT GPIOB

	#define GREEN_LED_PIN GPIO_Pin_15
	#define GREEN_LED_PORT GPIOB
#else
	#define BUZZ_PIN GPIO_Pin_15
	#define BUZZ_PORT GPIOB

	#define RED_LED_PIN GPIO_Pin_7
	#define RED_LED_PORT GPIOB

	#define GREEN_LED_PIN GPIO_Pin_5
	#define GREEN_LED_PORT GPIOB
#endif

typedef enum
{
	TYPE_KEEP_OFF = 0,
	TYPE_KEEP_ON = 1,
	TYPE_BLINK_DOUBLE = 2,
	TYPE_BLINK_FAST = 3,
	TYPE_BLINK_SLOW = 4,
	TYPE_BLINK_ONCE = 5
} BlinkType_T;

void GPIO_Init_Buzz();
void raise_beep_short();
void raise_double_beep();
void raise_long_beep();
void raise_over_speed_alarm(void);
void raise_long_run_alarm(void);
void raise_over_driving_in_day(void);
void raise_receive_data(void);
void raise_command_coming(void);

void turn_on_red_led();
void turn_off_red_led();

void turn_on_green_led();
void turn_off_green_led();

void blink_red_led_once();
void blink_green_led_once();

void blink_sd_data_coming();
void blink_green_led(BlinkType_T blinkType);
void blink_red_led(BlinkType_T blinkType);

void blink_valid_gps();
void blink_invalid_gps();
void blink_invalid_card();

void blink_no_x3_output();

void process_alarm_and_led(void);

#endif /* ALARM_H_ */
