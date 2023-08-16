#include "stdio.h"
#include "time.h"
#include "config.h"
#include "DataUtil.h"
#include "Util.h"
#include "main.h"
//#include "common.h"
#include "xprintf.h"

extern struct tm c_date_time;

unsigned int checkTripIdx = 0;
extern char buffer32[BUFFER_32];

/*
Read general configuration
*/
void read_device_config() {
	flash_read_config_info(&g_config);

	if(g_config.diffX < 3) {
		g_config.diffX = 5;

		if(g_config.timeout3D <= 1) {
			g_config.timeout3D = 3;
		}
	}

	if(g_config.diffY < 3) {
		g_config.diffY = 5;

		if(g_config.timeout3D <= 1) {
			g_config.timeout3D = 3;
		}
	}

	if(g_config.diffX > 30) {
		g_config.diffX = 5;
	}

	if(g_config.diffY > 30) {
		g_config.diffY = 5;
	}
}

/*
Write general configuration to ROM
*/
void write_device_config() {
	flash_write_config_info(&g_config);
}

void init_config() {
	read_device_config(); //read config from ROM
	write_default_config(); //write default config if not configured yet
}

void write_default_config() {
	//set default
	if(g_config.configState != 2) {
		//xprintf(, "1234567890");

		sprintf(g_config.deviceCode, "%s", "1234567890");

		g_config.emptyValue = 0;
		g_config.fullValue = 0;
		g_config.timeout3D = 3; //3 minutes
		g_config.outputMode = 0;
		g_config.diffX = 6;
		g_config.diffY = 6;
		g_config.interval = 1;

		g_config.configState = 2;

		write_device_config();

		xprintf("\r\nWrite default config.\r\n\r\n");
	} else {
		//xprintf("\r\nLoaded config.\r\n\r\n");
	}
}

extern char printLine[BUFFER_MSG];
extern char debugMark;

