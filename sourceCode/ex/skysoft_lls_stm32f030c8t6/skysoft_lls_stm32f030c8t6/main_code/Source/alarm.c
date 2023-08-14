#include "alarm.h"
#include "config.h"
#include "gpio.h"

BlinkType_T redLedBlinkType;
BlinkType_T greenLedBlinkType;

volatile uint32_t lastRaiseAlarm;
extern volatile uint32_t ulTickCount; //10 miliseconds
extern volatile uint32_t ulSecCount;

uint32_t nextGreenTick;
uint32_t nextRedTick;
uint32_t nextBeepTick;

typedef struct _BLINK_STEP {
	int duration;
	int type;
} BLINK_STEP;

BLINK_STEP greenLedStep[4];
BLINK_STEP redLedStep[4];
BLINK_STEP beepStep[4];

int greenLedStepIdx = 0;
int redLedStepIdx = 0;
int greenLedStepCount = 5;
int redLedStepCount = 5;
int beepStepIdx = -1;
int beepStepCount = 5;

void GPIO_Init_Buzz() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Configure the buzz */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Pin = BUZZ_PIN;
	GPIO_Init(BUZZ_PORT, &GPIO_InitStructure);

	/* Configure the RED LED */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Pin = RED_LED_PIN;
	GPIO_Init(RED_LED_PORT, &GPIO_InitStructure);

	/* Configure the GREAN LED */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Pin = GREEN_LED_PIN;
	GPIO_Init(GREEN_LED_PORT, &GPIO_InitStructure);
}
#if 0
void BuzzOff() {
	GPIO_ResetBits(BUZZ_PORT, BUZZ_PIN);
}

void BuzzOn() {
	GPIO_SetBits(BUZZ_PORT, BUZZ_PIN);
}

void RedLedOn() {
	GPIO_ResetBits(RED_LED_PORT, RED_LED_PIN);
}

void RedLedOff() {
	GPIO_SetBits(RED_LED_PORT, RED_LED_PIN);
}

void GreenLedOn() {
	GPIO_ResetBits(GREEN_LED_PORT, GREEN_LED_PIN);
}

void GreenLedOff() {
	GPIO_SetBits(GREEN_LED_PORT, GREEN_LED_PIN);
}
#endif
void raise_beep_short() {
	beepStep[0].duration = 30;
	beepStep[0].type = 1;
	beepStep[1].duration = 30;
	beepStep[1].type = 0;
	beepStepCount = 2;
	beepStepIdx = 0;
	nextBeepTick = 0;
}

void raise_double_beep() {
	beepStep[0].duration = 30;
	beepStep[0].type = 1;
	beepStep[1].duration = 20;
	beepStep[1].type = 0;
	beepStep[2].duration = 30;
	beepStep[2].type = 1;
	beepStep[3].duration = 10;
	beepStep[3].type = 0;
	beepStepCount = 4;
	beepStepIdx = 0;
	nextBeepTick = 0;
}

void raise_long_beep() {
	beepStep[0].duration = 50;
	beepStep[0].type = 1;
	beepStep[1].duration = 10;
	beepStep[1].type = 0;
	beepStepCount = 2;
	beepStepIdx = 0;
	nextBeepTick = 0;
}

void raise_over_speed_alarm(void) {

/*	if((ulSecCount - lastRaiseAlarm) > ALARM_RAISE_INTERVAL) {
		raise_double_beep();
		lastRaiseAlarm = ulSecCount;
	}*/
}

void raise_long_run_alarm(void) {
	/*if((ulSecCount - lastRaiseAlarm) > ALARM_RAISE_INTERVAL) {
		raise_long_beep();
		lastRaiseAlarm = ulSecCount;
	}*/
}

void raise_over_driving_in_day(void) {
	/*if((ulSecCount - lastRaiseAlarm) > ALARM_RAISE_INTERVAL) {
		raise_long_beep();
		lastRaiseAlarm = ulSecCount;
	}*/
}

void blink_green_led(BlinkType_T blinkType) {
	if(greenLedBlinkType == blinkType) {
		return;
	}
	greenLedBlinkType = blinkType;
	if(greenLedBlinkType == TYPE_BLINK_DOUBLE) {
		greenLedStep[0].duration = 10;
		greenLedStep[0].type = 1;
		greenLedStep[1].duration = 10;
		greenLedStep[1].type = 0;
		greenLedStep[2].duration = 10;
		greenLedStep[2].type = 1;
		greenLedStep[3].duration = 200;
		greenLedStep[3].type = 0;
		greenLedStepCount = 4;
		greenLedStepIdx = 0;
	} else if(greenLedBlinkType == TYPE_BLINK_FAST) {
		greenLedStep[0].duration = 10;
		greenLedStep[0].type = 1;
		greenLedStep[1].duration = 10;
		greenLedStep[1].type = 0;
		greenLedStepCount = 2;
		greenLedStepIdx = 0;
	} else if(greenLedBlinkType == TYPE_BLINK_SLOW) {
		greenLedStep[0].duration = 180;
		greenLedStep[0].type = 1;
		greenLedStep[1].duration = 50;
		greenLedStep[1].type = 0;
		greenLedStepCount = 2;
		greenLedStepIdx = 0;
	} else if(redLedBlinkType == TYPE_BLINK_ONCE) {
		greenLedStep[0].duration = 30;
		greenLedStep[0].type = 1;
		greenLedStep[1].duration = 20;
		greenLedStep[1].type = 0;
		greenLedStepCount = 2;
		greenLedStepIdx = 0;
	}
}

void blink_red_led(BlinkType_T blinkType) {
	if(redLedBlinkType == blinkType) {
		return;
	}
	redLedBlinkType = blinkType;
	if(redLedBlinkType == TYPE_BLINK_DOUBLE) {
		redLedStep[0].duration = 10;
		redLedStep[0].type = 1;
		redLedStep[1].duration = 10;
		redLedStep[1].type = 0;
		redLedStep[2].duration = 10;
		redLedStep[2].type = 1;
		redLedStep[3].duration = 200;
		redLedStep[3].type = 0;
		redLedStepCount = 4;
		redLedStepIdx = 0;
	} else if(redLedBlinkType == TYPE_BLINK_FAST) {
		redLedStep[0].duration = 10;
		redLedStep[0].type = 1;
		redLedStep[1].duration = 10;
		redLedStep[1].type = 0;
		redLedStepCount = 2;
		redLedStepIdx = 0;
	} else if(redLedBlinkType == TYPE_BLINK_SLOW) {
		redLedStep[0].duration = 180;
		redLedStep[0].type = 1;
		redLedStep[1].duration = 50;
		redLedStep[1].type = 0;
		redLedStepCount = 2;
		redLedStepIdx = 0;
	} else if(redLedBlinkType == TYPE_BLINK_ONCE) {
		redLedStep[0].duration = 30;
		redLedStep[0].type = 1;
		redLedStep[1].duration = 20;
		redLedStep[1].type = 0;
		redLedStepCount = 2;
		redLedStepIdx = 0;
	}
}
#if 0
void process_green_led() {
	if(greenLedBlinkType == TYPE_KEEP_ON) {
		GreenLedOn();
	} else if(greenLedBlinkType == TYPE_KEEP_OFF) {
		GreenLedOff();
	} else if(greenLedBlinkType == TYPE_BLINK_DOUBLE ||
			greenLedBlinkType == TYPE_BLINK_FAST ||
			greenLedBlinkType == TYPE_BLINK_SLOW ||
			greenLedBlinkType == TYPE_BLINK_ONCE) {
		if(ulTickCount > nextGreenTick) {
			if(greenLedStep[greenLedStepIdx].type) {
				GreenLedOn();
			} else {
				GreenLedOff();
			}
			nextGreenTick = ulTickCount + greenLedStep[greenLedStepIdx].duration;
			if((greenLedStepIdx + 1) >= greenLedStepCount && greenLedBlinkType == TYPE_BLINK_ONCE) {
				greenLedBlinkType = TYPE_KEEP_OFF;
			} else {
				greenLedStepIdx = (greenLedStepIdx + 1) % greenLedStepCount;
			}
		}
	} else {
		GreenLedOff();
	}
}
#endif
void process_red_led() {
	if(redLedBlinkType == TYPE_KEEP_ON) {
		RedLedOn();
	} else if(redLedBlinkType == TYPE_KEEP_OFF) {
		RedLedOff();
	} else if(redLedBlinkType == TYPE_BLINK_DOUBLE ||
			redLedBlinkType == TYPE_BLINK_FAST ||
			redLedBlinkType == TYPE_BLINK_SLOW ||
			redLedBlinkType == TYPE_BLINK_ONCE) {
		if(ulTickCount > nextRedTick) {
			if(redLedStep[redLedStepIdx].type) {
				RedLedOn();
			} else {
				RedLedOff();
			}
			nextRedTick = ulTickCount + redLedStep[redLedStepIdx].duration;
			if((redLedStepIdx + 1) >= redLedStepCount && redLedBlinkType == TYPE_BLINK_ONCE) {
				redLedBlinkType = TYPE_KEEP_OFF;
			} else {
				redLedStepIdx = (redLedStepIdx + 1) % redLedStepCount;
			}
		}
	} else {
		RedLedOff();
	}
}

void process_buzz() {
	if(beepStepIdx != -1 && ulTickCount > nextBeepTick && beepStepIdx < beepStepCount) {
		if(beepStep[beepStepIdx].type) {
			BuzzOn();
		} else {
			BuzzOff();
		}
		nextBeepTick = ulTickCount + beepStep[beepStepIdx].duration;
		beepStepIdx++;
	} else if (beepStepIdx != -1 && beepStepIdx < beepStepCount) { //in beep

	} else { //out of beep

	}
}

void turn_on_red_led() {
	blink_red_led(TYPE_KEEP_ON);
}

void turn_off_red_led() {
	blink_red_led(TYPE_KEEP_OFF);
}

void turn_on_green_led() {
	blink_green_led(TYPE_KEEP_ON);
}

void turn_off_green_led() {
	blink_green_led(TYPE_KEEP_OFF);
}

void blink_sd_data_coming() {
	blink_red_led(TYPE_BLINK_FAST);
}

void blink_valid_gps() {
	blink_green_led(TYPE_KEEP_ON);
}

void blink_invalid_gps() {
	blink_green_led(TYPE_BLINK_SLOW);
}

void blink_invalid_card() {
	blink_green_led(TYPE_BLINK_DOUBLE);
}

void blink_no_x3_output() {
	blink_green_led(TYPE_KEEP_OFF);
}

void blink_red_led_once() {
	blink_red_led(TYPE_BLINK_ONCE);
}

void blink_green_led_once() {
	blink_green_led(TYPE_BLINK_ONCE);
}

void raise_command_coming() {
	raise_beep_short();
	blink_red_led_once();
}

void process_alarm_and_led(void) {
	process_red_led();
	//process_green_led();
	process_buzz();
}
