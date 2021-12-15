#ifndef __BSP_WIFI_H__
#define __BSP_WIFI_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include "sdkconfig.h"
#include  <stdint.h>
#include  <string.h>
/*------------------------------Define----------------------------------*/
typedef struct
{
    uint8_t mac[6];
    int8_t rssi;
}bsp_wifi_t;

#define BSP_WIFI_SCAN_LIST_SIZE 12
/*------------------------------Exertn----------------------------------*/
void bsp_wifi_init(void);
#endif // !__USER_H
