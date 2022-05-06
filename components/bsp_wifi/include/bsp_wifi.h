#ifndef __BSP_AP_H__
#define __BSP_AP_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include <stdint.h>
#include <stdbool.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
typedef struct
{
    uint16_t id;
    int8_t rssi;
    uint8_t mac[6];
} bsp_wifi_t;

typedef struct
{
    uint8_t mac[6];
} bsp_whitelist_t;
/*------------------------------Exertn----------------------------------*/
void bsp_ap_init(uint8_t *ssid, uint8_t *password, uint8_t channel, uint8_t conn_limit);
void bsp_sta_init(uint8_t *ssid, uint8_t *password);
void bsp_wifi_scan(void);
void bsp_server_start(void);
// void stop_webserver(httpd_handle_t server);
#endif // !BSP_AP_H__