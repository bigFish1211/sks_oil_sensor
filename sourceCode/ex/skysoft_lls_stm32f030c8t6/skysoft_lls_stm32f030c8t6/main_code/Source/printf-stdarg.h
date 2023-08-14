/*
 * printf-stdarg.h
 *
 *  Created on: Feb 17, 2022
 *      Author: GiangNT
 */

#ifndef PRINTF_STDARG_H_
#define PRINTF_STDARG_H_

int usnprintf(char *pcBuf, unsigned long ulSize, const char *pcString, ...);
uvsnprintf(char *pcBuf, unsigned long ulSize, const char *pcString, va_list vaArgP);
#endif /* PRINTF_STDARG_H_ */
