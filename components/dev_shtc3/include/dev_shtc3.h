#ifndef __DEV_SHTC3_H__
#define __DEV_SHTC3_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include <stdint.h>
#include <stdbool.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/

/*------------------------------Exertn----------------------------------*/
void dev_shtc3_measurement(float *temp, float *humi);

#endif // !DEV_SHTC3_H__
