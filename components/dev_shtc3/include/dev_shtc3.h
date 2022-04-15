#ifndef __DEV_SHTC3_H__
#define __DEV_SHTC3_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include <stdint.h>
#include <stdbool.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
#define SHTC3_ADDR  0x70
#define CRC_POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

#define SHTC3_RAW_DATA_SIZE 6
/*------------------------------Exertn----------------------------------*/
void dev_shtc3_measurement(float *temp, float *humi);

#endif // !DEV_SHTC3_H__
