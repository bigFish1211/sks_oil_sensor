/*
 * download.h
 *
 *  Created on: Mar 22, 2022
 *      Author: icepe
 */

#ifndef INC_DOWNLOAD_H_
#define INC_DOWNLOAD_H_

int getFirmWareInfo(void);
int downLoadDataInNewFirmWare(uint32_t size);
void jumpToApplication(void);

#endif /* INC_DOWNLOAD_H_ */
