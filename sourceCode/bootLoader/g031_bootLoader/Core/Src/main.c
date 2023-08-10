#include <stdio.h>
#include <stdint.h>
#include "config.h"
#include  "global.h"
#include "drv_io.h"
#include "dvr_crc.h"
#include "define.h"
volatile uint32_t ulMiliCount = 0;
volatile uint32_t ulSecCount = 0;

static uint32_t ulTickCount = 0;
static int setUp(void);

extern mainTask(void);
int main(void) {
	setUp();
	mainTask();
	while (1) {

	}
	return 1;
}

static int setUp(void) {
	systemInit();
	usart_x0_init(115200);
	CRC_ClockInit();
//	xdev_out(SerialPutChar);
}

void SysTick_Handler(void) {
	ulMiliCount++;
	ulTickCount++;
	if (ulTickCount > 1000) {
		ulSecCount++;
	}
}
