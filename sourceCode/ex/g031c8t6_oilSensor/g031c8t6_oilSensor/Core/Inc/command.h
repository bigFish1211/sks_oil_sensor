/*
 * command.h
 *
 *  Created on: Sep 6, 2022
 *      Author: icepe
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include <stdint.h>

#define SKS_CMD_SOURCE_COM1 1
#define SKS_CMD_SOURCE_COM2 2

int server_on_command(int from, uint8_t * data, int size);

#endif /* INC_COMMAND_H_ */
