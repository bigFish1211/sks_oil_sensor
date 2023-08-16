#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdint.h>
#include  <stdbool.h>


#define DEBUG 								1
#define SEND_SERVER_SCHEDULE	1000//ms

extern volatile uint32_t ulSecCount;
extern volatile uint32_t ulMiliCount;


extern volatile char buff_cmd[256];
extern int buff_idx;

typedef enum _ERROR_STATE {
	ERROR_STATE_NORMAL = 0,
	ERROR_STATE_ZERO,
	ERROR_STATE_OVER_EMPTY,
	ERROR_STATE_BELOW_FULL,
	ERROR_STATE_NOT_SET_FULL_EMTY,
} ERROR_STATE;

#endif
