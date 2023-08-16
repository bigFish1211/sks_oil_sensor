/*
 * report.c
 *
 *  Created on: Aug 29, 2012
 *      Author: Admin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "printf-stdarg.h"
#include "xprintf.h"

#include <time.h>
#include "DataUtil.h"
#include "command.h"
#include "main.h"
//#include "alarm.h"
#include "config.h"
//#include "Util.h"

extern volatile uint32_t ulSecCount;

typedef int (*SKS_CMD_HANDLER)(int from, char* command, int argc, char** argv, int size, int total);

typedef struct _SKS_HANDLER {
	char* cmd;
	SKS_CMD_HANDLER handler;
	unsigned short dis_mask; //Disable handle from source mask
	unsigned short dis_pass; //This command can execute without pass
} SKS_HANDLER;

char *HexChar = "1234567890ABCDEF";
static int response = 0;

//void turn_debug(char mark);

void append_int(char* str, uint8_t len, uint32_t val) {
	uint8_t i;
	char *ptr = str + strlen(str);
	for (i = 1; i <= len; i++) {
		ptr[len - i] = (uint8_t) ((val % 10UL) + '0');
		val /= 10;
	}
	ptr[i - 1] = '\0';
}

void append_hex(char* str, uint8_t val) {
	char *ptr = str + strlen(str);
	ptr[0] = HexChar[val >> 4];
	ptr[1] = HexChar[val & 0xF];
	ptr[2] = 0;
}

void append_hex_long(char* str, long val) {
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

int cmd_response(char* buff, int size, char *cmd, const char *fmt, ...)
{
	int n = 0;
    va_list ap;
    n = usnprintf(buff, size, "*SS,%s,V9,%s,", g_config.deviceCode, cmd);
    va_start(ap, fmt);
    char *bufftmp = buff + (n);
    n = uvsnprintf(bufftmp, size-(n+1), (char *) fmt, ap );
    va_end(ap);
    bufftmp[n] = '#';
    n++;
    bufftmp[n] = '\r';
    n++;
    bufftmp[n] = '\n';
    return n+1;
}

int append_sign(char* buff) {
	strcpy(buff, "*SS,");
	strcat(buff, (char*)g_config.deviceCode);
	return 1;
}

int append_time(char* buff, time_t ti) {
	time_t now = ti;
	struct tm *tm = gmtime(&now);
	char *ptr = buff + strlen(buff);
	append_int(ptr, 4, tm->tm_year + 1900);
	strcat(ptr, "-");
	append_int(ptr, 2, tm->tm_mon + 1);
	strcat(ptr, "-");
	append_int(ptr, 2, tm->tm_mday);
	strcat(ptr, " ");
	append_int(ptr, 2, tm->tm_hour);
	strcat(ptr, ":");
	append_int(ptr, 2, tm->tm_min);
	strcat(ptr, ":");
	append_int(ptr, 2, tm->tm_sec);
	return 1;
}

int append_v9_command(char *buff, char* command) {
	strcat(buff, ",V9,");
	strcat(buff, command);
	return 1;
}

int next_char(char* data, char sep, int max) {
	char *ptr = strchr(data, sep);
	if (ptr) {
		return (ptr - data) >= max ? max : (ptr - data);
	} else {
		return 0;
	}
}

static char Response[256];
void create_v3(int blockno) {
	append_sign(Response);
	strcat(Response, ",V3,");
	if (blockno >= 10000) {
		append_int(Response, 5, blockno);
	} else if (blockno >= 1000) {
		append_int(Response, 4, blockno);
	} else if (blockno >= 100) {
		append_int(Response, 3, blockno);
	} else if (blockno >= 10) {
		append_int(Response, 2, blockno);
	} else {
		append_int(Response, 1, blockno);
	}
}

int write_flash = 0;

static int sks_cmd_set_debug(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;
	if (argv[4] == NULL)
		return size;
	set = atoi(argv[4]);

	g_appStats.debugOn = set;

	cmd_response(Response, sizeof(Response), command, "%d,%d", set, g_appStats.debugOn);

	response = 1;

	return size;
}

static int sks_cmd_set_full(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;
	if (argv[4] == NULL)
		return size;
	set = atoi(argv[4]);

	if(set == 1) {
		if(argv[5] != NULL) {
			g_config.fullValue = atoi(argv[5]);
			if(g_config.fullValue == 0) {
				g_config.fullValue = g_appStats.frequency;
			}
		} else {
			g_config.fullValue = g_appStats.frequency;
		}
		write_flash = 1;
	}

	cmd_response(Response, sizeof(Response), command, "%d,%d,%d", set, g_config.emptyValue, g_config.fullValue);

	//raise_command_coming();

	response = 1;
	return size;
}

static int sks_cmd_set_empty(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;
	if (argv[4] == NULL)
		return size;
	set = atoi(argv[4]);

	if(set == 1) {
		if(argv[5] != NULL) {
			g_config.emptyValue = atoi(argv[5]);
			if(g_config.emptyValue == 0) {
				g_config.emptyValue = g_appStats.frequency;
			}
		} else {
			g_config.emptyValue = g_appStats.frequency;
		}
		write_flash = 1;
	}

//	append_sign(Response);
//	append_v9_command(Response, command);
//	strcat(Response, ",");
//	append_int(Response, 1, set);
//	strcat(Response, ",");
//	append_number(Response, g_config.emptyValue);
//	strcat(Response, ",");
//	append_number(Response, g_config.fullValue);
//	strcat(Response, "#\r\n");

	cmd_response(Response, sizeof(Response), command, "%d,%d,%d", set, g_config.emptyValue, g_config.fullValue);

	//raise_command_coming();

	response = 1;
	return size;
}

static int sks_cmd_disable_ema(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;

	if (argv[4] == NULL)
		return size;

	set = atoi(argv[4]);

	if(set == 1) {
		if(argv[5] != NULL) {
			int tmp = atoi(argv[5]);
			g_config.disableEma = (tmp > 0)?1:0;
		}

		write_flash = 1;
	}

	cmd_response(Response, sizeof(Response), command, "%d,%d", set, g_config.disableEma);

	//raise_command_coming();

	response = 1;
	return size;
}

static int sks_cmd_interval(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;

	if (argv[4] == NULL)
		return size;

	set = atoi(argv[4]);

	if(set == 1) {
		if(argv[5] != NULL) {
			g_config.interval = atoi(argv[5]);
		}

		write_flash = 1;
	}

	cmd_response(Response, sizeof(Response), command, "%d,%d", set, g_config.interval);

	//raise_command_coming();

	response = 1;
	return size;
}

static int sks_cmd_config(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;
	if (argv[4] == NULL)
		return size;

	set = atoi(argv[4]);

	if(set == 1) {
		if(argv[5] != NULL) {
			g_config.timeout3D = atoi(argv[5]);
		}

		if(argv[6] != NULL) {
			g_config.filterMode = atoi(argv[6]);
		}

		if(argv[7] != NULL) {
			int diffX = atoi(argv[7]);
			if(diffX >= 3 && diffX <= 30) {
				g_config.diffX = diffX;
			}
		}

		if(argv[8] != NULL) {
			int diffY = atoi(argv[8]);
			if(diffY >= 3 && diffY <= 30) {
				g_config.diffY = diffY;
			}
		}

		write_flash = 1;
	}

//	append_sign(Response);
//	append_v9_command(Response, command);
//	strcat(Response, ",");
//	append_int(Response, 1, set);
//	strcat(Response, ",");
//	append_int(Response, 2, g_config.timeout3D);
//	strcat(Response, ",");
//	append_int(Response, 1, g_config.filterMode);
//	strcat(Response, ",");
//	append_int(Response, 2, g_config.diffX);
//	strcat(Response, ",");
//	append_int(Response, 2, g_config.diffY);
//	strcat(Response, "#\r\n");

	cmd_response(Response, sizeof(Response), command, "%d,%d,%d,%d,%d", set, g_config.timeout3D, g_config.filterMode, g_config.diffX, g_config.diffY);

	response = 1;
	return size;
}

static int sks_cmd_serial(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;
	if (argv[4] == NULL)
		return size;

	set = atoi(argv[4]);

	if(set == 1) {
		memset(g_config.deviceCode, 0, sizeof(g_config.deviceCode));
		strncpy(g_config.deviceCode, argv[5], next_char(argv[5], '#', DEVICE_CODE_LEN));

		write_flash = 1;
	}

//	append_sign(Response);
//	append_v9_command(Response, command);
//	strcat(Response, ",");
//	append_int(Response, 1, set);
//	strcat(Response, ",");
//	strcat(Response, g_config.deviceCode);
//	strcat(Response, "#\r\n");

	cmd_response(Response, sizeof(Response), command, "%d,%s", set, g_config.deviceCode);

	response = 1;
	return size;
}

static int sks_cmd_reset(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;
	if (argv[4] == NULL)
		return size;

	set = atoi(argv[4]);

	if(set == 1) {
		g_appStats.resetNow = ulSecCount;
	}

	cmd_response(Response, sizeof(Response), command, "%d", set);

	response = 1;
	return size;
}

static int sks_cmd_info(int from, char* command, int argc, char** argv, int size, int total) {
	//G9, debug setting
	//*XX,YYYYYYYYYY,SF,HHMMSS,S#
	char set = 0;
	if (argv[4] == NULL)
		return size;
	set = atoi(argv[4]);

	if(set == 1) {
	}

	cmd_response(Response, sizeof(Response), command, "%d,%d,%d,%d,%d,%s,%s", set, g_config.interval, g_config.filterMode, g_config.emptyValue, g_config.fullValue, g_config.deviceCode, APP_VERSION);

	//raise_command_coming();

	response = 1;
	return size;
}

SKS_HANDLER handlers[] = {
		{ "CFG", sks_cmd_config }, //get config
		{ "INFO", sks_cmd_info }, //get config
		{ "INTERVAL", sks_cmd_interval }, //get config
		{ "EMA", sks_cmd_disable_ema }, //get config
		{ "SN", sks_cmd_serial }, //set serial
		{ "RESET", sks_cmd_reset }, //set serial
		{ "SF", sks_cmd_set_full }, //get config
		{ "SE", sks_cmd_set_empty }, //get config
		{ "DEBUG", sks_cmd_set_debug }, //get config
		{ NULL, NULL } };

int server_on_command(int from, char* data, int size) {
	#define SKS_MAX_CMD_LEN 10
	#define SKS_MAX_ARGV 	25
	int result = 0;
	int field = 0;
	int ftmp;
	int j = 0;
	char *ptr = data;

	char command[SKS_MAX_CMD_LEN + 1] = { 0 };

	int endPos = 0;
	int i = 0;
	char item_buff[20] = { 0 };
	char* arg[SKS_MAX_ARGV];
	for (i = 0; i < size; i++) {
		if ((data[i] == '#') || (data[i] == '\n') || (data[i] == '\0')) {
			endPos = i + 1;
			break;
		}
	}

	//xprintf("come here: %d\r\n", 1);

	if (endPos == 0)
		return 0; //Waiting for end command sign

	//xprintf("come here: %d, endPos: %d\r\n", 2, endPos);

	for (int i = 0; i < endPos; i++) {
		if ((data[i] == ',') || (data[i] == '\n') || (data[i] == '#') || (data[i] == 0x00)) //complete one item
		{
			//xprintf("FIELD0 %d j=%d DATA %s\r\n", field, j, item_buff);

			if (j > 0) {
				//xprintf("FIELD1 %d DATA %s, ptr=%s\r\n", field, item_buff, ptr);

				//if(*(uint32_t*)(ptr) == 0x2C53532A) //*SS,
				if(strncmp(ptr, "*SS,", 4) == 0) //*SS,
				{
					field = 0;
				}
				ftmp = field;

				//xprintf("FIELD %d DATA %s\r\n", field, item_buff);

				arg[field % SKS_MAX_ARGV] = ptr;
				switch (ftmp) {
				case 0:
					if(strncmp(ptr, "*SS,", 4) != 0) //*SS,
						return -1; //*SS,
					break;
				case 1:
					break;
				case 2:
					memcpy(command, ptr, j < SKS_MAX_CMD_LEN ? j : SKS_MAX_CMD_LEN);
					break;
				case 3:
					break;
				default:
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

			//xprintf("Item buff %s\r\n", item_buff);
		}
	}

	//xprintf("come here: %d\r\n", 3);

	if (field > SKS_MAX_ARGV)
		return endPos; //Too many param

	//xprintf("come here: %d\r\n", 4);

	if (field <= 3)
		return endPos; //Too few param

	//xprintf("server_on_command: %s, command: %s \r\n", data, command);

	i = 0;
	result = -1;
	response = 0;
	while(handlers[i].cmd) {
		if (strcmp(command, handlers[i].cmd) == 0) {
			//sks_log(LOG_INFO, "COMMAND A %s handlers[%d].cmd=%s", command, i, handlers[i].cmd);
			if (!(handlers[i].dis_mask & from)) //Chi cho phep gui lenh tu nguon
			{
				write_flash = 0;
				memset(Response, 0, 200);

				result = handlers[i].handler(from, command, field, arg, endPos, size);
				//sks_log(LOG_INFO, "***********===************COMMAND B%s handlers[%d].cmd=%s, total %d result:%d\r\n", command, i, handlers[i].cmd, size, result);

				if(write_flash) {
					write_config();
				}
			}
			break;
		}
		i++;
	}

	if (result > 0) {
		if (response) {
			if (from == SKS_CMD_SOURCE_COM1) {
				xprintf(Response);
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
