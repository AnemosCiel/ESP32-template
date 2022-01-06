#ifndef __BSP_WIFI_H__
#define __BSP_WIFI_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include "sdkconfig.h"
#include <stdint.h>
#include <string.h>
/* rtos */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
/*------------------------------Define----------------------------------*/
#define WIFI_INFO           0

#define BSP_WIFI_MAC_SIZE   6

#define BSP_WIFI_SCAN_LIST_SIZE     12
#define BSP_WIFI_WHITE_LIST_SIZE    12

typedef struct
{
    uint16_t id;
    int8_t rssi;
    uint8_t mac[BSP_WIFI_MAC_SIZE];
} bsp_wifi_t;

typedef struct
{
    uint8_t mac[BSP_WIFI_MAC_SIZE];
} bsp_whitelist_t;

/*------------------------------Exertn----------------------------------*/
extern bsp_wifi_t wifi_info[BSP_WIFI_SCAN_LIST_SIZE];
extern bsp_whitelist_t white_list[BSP_WIFI_WHITE_LIST_SIZE];
extern SemaphoreHandle_t wifiSemphr;

void bsp_wifi_init( void );
#endif // !__USER_H
