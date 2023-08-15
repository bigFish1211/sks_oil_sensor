/*
 * command.c
 *
 *  Created on: Sep 6, 2022
 *      Author: icepe
 */

#include "command.h"
#include "EMA.h"
#include "define.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef int (*SKS_CMD_HANDLER)(int from, char *command, int argc, char **argv,
		int size, int total);

typedef struct _SKS_HANDLER {
	char *cmd;
	SKS_CMD_HANDLER handler;
	unsigned short dis_mask; //Disable handle from source mask
	unsigned short dis_pass; //This command can execute without pass
} SKS_HANDLER;

char *HexChar = "1234567890ABCDEF";
static int response = 0;

static void append_int(char *str, uint8_t len, uint32_t val);
static void append_hex(char *str, uint8_t val);
static void append_hex_long(char *str, long val);
int next_char(char *data, char sep, int max);
static int sks_cmd_set_full(int from, char *command, int argc, char **argv,
		int size, int total);
static int sks_cmd_set_empty(int from, char *command, int argc, char **argv,
		int size, int total);
static int sks_cmd_set_debug(int from, char *command, int argc, char **argv,
		int size, int total);

static void append_int(char *str, uint8_t len, uint32_t val) {
	uint8_t i;
	char *ptr = str + strlen(str);
	for (i = 1; i <= len; i++) {
		ptr[len - i] = (uint8_t) ((val % 10UL) + '0');
		val /= 10;
	}
	ptr[i - 1] = '\0';
}

static void append_hex(char *str, uint8_t val) {
	char *ptr = str + strlen(str);
	ptr[0] = HexChar[val >> 4];
	ptr[1] = HexChar[val & 0xF];
	ptr[2] = 0;
}

static void append_hex_long(char *str, long val) {
	char *ptr = str + strlen(str);
	ptr[0] = HexChar[val >> 28];
	ptr[1] = HexChar[(val >> 24) & 0xF];
	ptr[2] = HexChar[(val >> 20) & 0xF];
	ptr[3] = HexChar[(val >> 16) & 0xF];
	ptr[4] = HexChar[(val >> 12) & 0xF];
	ptr[5] = HexChar[(val >> 8) & 0xF];
	ptr[6] = HexChar[(val >> 4) & 0xF];
	ptr[7] = HexChar[val & 0xF];
	ptr[8] = 0x0;
}

int next_char(char *data, char sep, int max) {
	char *ptr = strchr(data, sep);
	if (ptr) {
		return (ptr - data) >= max ? max : (ptr - data);
	} else {
		return 0;
	}
}

static char Response[256];

static int sks_cmd_set_full(int from, char *command, int argc, char **argv,
		int size, int total) {
	char set = 0;
	if (argv[4] == NULL)
		return size;
	set = atoi(argv[4]);
	if (set) {
		printf("SKS_OIL_SENSOR SET FULL\r\n");
		g_appConfig.wirteConfigNow = 1;
		sensorConfig.full = (uint32_t)SensorEMA10.lastEMA;//(uint32_t)getAverage();
	}
	response = 0;
	return size;
}

static int sks_cmd_set_empty(int from, char *command, int argc, char **argv,
		int size, int total) {
	char set = 0;
	if (argv[4] == NULL)
		return size;
	set = atoi(argv[4]);
	if (set) {
		printf("SKS_OIL_SENSOR SET EMPTY\r\n");
		g_appConfig.wirteConfigNow = 1;
		sensorConfig.empty = (uint32_t)SensorEMA10.lastEMA;//(uint32_t)getAverage();
	}

	response = 0;
	return size;
}

static int sks_cmd_set_debug(int from, char *command, int argc, char **argv,
		int size, int total) {
	char set = 0;
	if (argv[4] == NULL)
		return size;
	set = atoi(argv[4]);
	if (set) {
		printf("SKS_OIL_SENSOR SET DEBUG ON\r\n");
		g_appConfig.debug = 1;
	}
	else{
		printf("SKS_OIL_SENSOR SET DEBUG ON\r\n");
		g_appConfig.debug = 0;
	}

	response = 0;
	return size;
}

static int sks_cmd_set_EMA(int from, char *command, int argc, char **argv,
	int size, int total) {
char set = 0;
uint8_t ema_sel;
if (argv[4] == NULL)
	return size;
set = atoi(argv[4]);
ema_sel = atoi(argv[5]);
if (set) {

	switch (ema_sel) {
	case EMA5:
		printf("SKS_OIL_SENSOR SET EMA5\r\n");
		break;
	case EMA10:
		printf("SKS_OIL_SENSOR SET EMA10\r\n");
		break;
	case EMA15:
		printf("SKS_OIL_SENSOR SET EMA15\r\n");
		break;
	case EMA30:
		printf("SKS_OIL_SENSOR SET EMA30\r\n");
		break;
	case EMA50:
		printf("SKS_OIL_SENSOR SET EMA50\r\n");
		break;
	default:
		break;
	}
	sensorConfig.ema = ema_sel;
	g_appConfig.wirteConfigNow = 1;
}

response = 0;
return size;
}

SKS_HANDLER handlers[] = { 	{ "SF", sks_cmd_set_full }, // set full
							{ "SE", sks_cmd_set_empty }, //set empty
							{ "DEBUG", sks_cmd_set_debug }, //set empty
							{ "EMA", sks_cmd_set_EMA }, //set empty
							{ NULL, NULL } };


int server_on_command(int from, uint8_t *data, int size) {
#define SKS_MAX_CMD_LEN 10
#define SKS_MAX_ARGV 	25
	int result = 0;
	int field = 0;
	int ftmp;
	int j = 0;
	char *ptr = data;
	//char invalide_code = 0;
	char command[SKS_MAX_CMD_LEN + 1] = { 0 };
	//*XX,YYYYYYYYYY,CMD,VVVVVV,PARA1,PARA2,�#
	//sks_log(LOG_INFO, "Process COMMAND size=%d data=%s\r\n", size, data);
	int endPos = 0;
	int i = 0;
	char item_buff[20] = { 0 };
	char *arg[SKS_MAX_ARGV];
	for (i = 0; i < size; i++) {
		if ((data[i] == '#') || (data[i] == '\n') || (data[i] == '\0')) {
			endPos = i + 1;
			break;
		}
	}
	if (endPos == 0)
		return 0; //Waiting for end command sign
	//xprintf("endPos: %d \n\r",endPos);
	for (int i = 0; i < endPos; i++) {

		if ((data[i] == ',') || (data[i] == '\n') || (data[i] == '#')
				|| (data[i] == 0x00)) //complete one item
				{

			if (j > 0) {

				ftmp = field;
				arg[field % SKS_MAX_ARGV] = ptr;
				switch (ftmp) {
				case 0:
					if (*(uint32_t*) (ptr) != 0x2C53532A) {

						return -1; //*SS,
					}
					break;
				case 1:
					break;
				case 2:
					memcpy(command, ptr,
							j < SKS_MAX_CMD_LEN ? j : SKS_MAX_CMD_LEN);
					break;
				case 3:
					//if (memcmp(ptr, g_config.ADMINCODE, 6)) invalide_code |= 0x2; //Invalid AdminCode
					break;
				default:
					//return endPos;
					break;
				}
			}
			field++;
			if (field >= SKS_MAX_ARGV)
				break;
			j = 0; //reset index
			ptr = data + i + 1;
			arg[field % SKS_MAX_ARGV] = ptr;
		} else {
			item_buff[j % sizeof(item_buff)] = data[i];
			j++;
			item_buff[j] = 0x00;
		}
	}

	if (field > SKS_MAX_ARGV)
		return endPos; //Too many param
	if (field <= 3)
		return endPos; //Too few param

	i = 0;
	result = -1;
	response = 0;
	while (handlers[i].cmd) {
		if (strcmp(command, handlers[i].cmd) == 0) {
			if (!(handlers[i].dis_mask & from)) //Chi cho phep gui lenh tu nguon
			{
				memset(Response, 0, 200);
				result = handlers[i].handler(from, command, field, arg, endPos,
						size);
			}
			break;
		}
		i++;
	}

	if (result > 0) {
		if (response) {
			if (from == SKS_CMD_SOURCE_COM1) {
				//xprintf(Response);
			} else if (from == SKS_CMD_SOURCE_COM2) {
				//ExtSerial_PutString(Response);
			} else {
			}
		}
	} else if (result < 0) {
		result = endPos;
	}
	return result;
}
