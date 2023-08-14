#include <stdio.h>
#include <stdint.h>
#include "config.h"
#include  "global.h"
#include "drv_io.h"
#include "dvr_crc.h"
#include "define.h"
#include "main.h"
#include "xprintf.h"
#include "dvr_uart.h"
#include "system_g031.h"

volatile uint32_t ulMiliCount = 0;
volatile uint32_t ulSecCount = 0;

static uint32_t ulTickCount = 0;
static uint32_t lastSendData = 0;
static int setUp(void);

extern void mainTask(void);
int main(void) {
	setUp();
	mainTask();
	while (1) {
		if ((uint32_t) (ulMiliCount - lastSendData) >= 1000) {
			lastSendData = ulMiliCount;
			xprintf("hello\r\n");
		}
	}
	return 1;
}

static int setUp(void) {
	int ret = 0;
	systemInit();
	usart_x0_init(115200);
	CRC_ClockInit();
	xdev_out(SerialPutChar);
	return ret;
}

void SysTick_Handler(void) {
	ulMiliCount++;
	ulTickCount++;
	if (ulTickCount > 1000) {
		ulTickCount =0;
		ulSecCount++;
	}
}
