/*
 * log.h
 *
 *  Created on: Sep 5, 2023
 *      Author: icepe
 */

#ifndef LOG_H_
#define LOG_H_
#include "stdio.h"
#include "dvr_uart.h"


//#endif /* LOG_H_ */
//define PRINTF(...) do\
//                       {\
//                            sprintf(buf_print_1,##__VA_ARGS__);\
//                            U1print(buf_print_1);\
//                        }\
//                    while(false);
//#define LOG  PRINTF
//#else
//#endif

#define CONSOLE_LOG 		1
#if CONSOLE_LOG

#define log(fmt, ...) do {                                    \
                          sprintf(buf_print_1,##__VA_ARGS__); \
                          fprintf(logfile, fmt, __VA_ARGS__); \
                      } while (0)

#else
#define log(fmt, ...)
#endif
