#ifndef __BSP_NOW_H__
#define __BSP_NOW_H__
/* Note: 
ESP-NOW Data is transmitted using action frames from each vendor, with a default bit rate of 1 Mbps. The action frame format for each vendor is:

-----------------------------------------------------------------------------------------
|MAC Header | Classification Code | Organization Identifier | Random Value | Vendor Specific Content | FCS |
-----------------------------------------------------------------------------------------
   24 bytes         1 byte                  3 bytes             4 bytes             7~255 bytes         4 bytes

    Sort Code: The Sort Code field can be used to indicate the category of each supplier (eg 127).
    Organization Identifier: The Organization Identifier contains a unique identifier (such as 0x18fe34), which is the first three bytes of the Espressif-assigned MAC address.
    Random value: Prevents replay attacks.
    Vendor-specific content: Vendor-specific content contains vendor-specific fields as follows:

----------------------------------------------------------------------------------------
|  Element ID | Length | Organization Identifier | Type | Version | Body |
----------------------------------------------------------------------------------------
   1 bytes     1 bytes          3 bytes          1 bytes 1 bytes  0～250 bytes

    Element ID: The Element ID field can be used to indicate a vendor-specific element.
    Length: The length is the total length of the organization identifier, type, version, and body.
    Organization Identifier: The Organization Identifier contains a unique identifier (such as 0x18fe34), which is the first three bytes of the Espressif-assigned MAC address.
    Type: The Type field is set to 4, which stands for ESP-NOW.
    Version: The Version field is set to the version of ESP-NOW.
    Body: The body contains ESP-NOW data.
    Since ESP-NOW is connectionless, the MAC header is slightly different from a standard frame. The FromDS and ToDS bits of the FrameControl field are both 0.
    The first address field is used to configure the destination address. The second address field is used to configure the source address.
    The third address field is used to configure the broadcast address (0xff:0xff:0xff:0xff:0xff:0xff).
*/
/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
#include  <stdbool.h>
/*----------driver---------*/
#include "esp_now.h"
/*------------------------------Define----------------------------------*/
#define BSP_NOW_MASTER 1

#if BSP_NOW_MASTER
#define BSP_ESPNOW_WIFI_MODE WIFI_MODE_AP
#define BSP_ESPNOW_WIFI_IF   ESP_IF_WIFI_AP
#else
#define BSP_ESPNOW_WIFI_MODE WIFI_MODE_STA
#define BSP_ESPNOW_WIFI_IF   ESP_IF_WIFI_STA
#endif

#define BSP_ESPNOW_CHANNEL  1

#define BSP_ESPNOW_PMK "pmk1234567890123" // ESPNOW primary master use. The length of ESPNOW primary master must be 16 bytes.
#define BSP_ESPNOW_LMK "lmk1234567890123" // ESPNOW primary master use. The length of ESPNOW local master must be 16 bytes.

#define BSP_ESPNOW_MACADDR_SIZE 6
#define BSP_ESPNOW_QUEUE_SIZE   6

enum {
    ESPNOW_DATA_BROADCAST,
    ESPNOW_DATA_UNICAST,
    ESPNOW_DATA_MAX,
};

/* User defined field of ESPNOW data in this example. */
typedef struct {
    uint8_t type;                         //Broadcast or unicast ESPNOW data.
    uint8_t state;                        //Indicate that if has received broadcast ESPNOW data or not.
    uint8_t *data;                      //Buffer pointing to ESPNOW data.
    int len;                              //Length of ESPNOW data to be sent, unit: byte.
    uint16_t seq_num;                     //Sequence number of ESPNOW data.
    uint16_t crc;                         //CRC16 value of ESPNOW data.
    uint32_t magic;                       //Magic number which is used to determine which device to send unicast ESPNOW data.
    uint8_t payload[0];                   //Real payload of ESPNOW data.
    uint8_t sender_mac[6];   //MAC address of target device.
}  espnow_recv_packet_t;

/* Parameters of sending ESPNOW data. */
typedef struct {
    uint8_t status;                        //Indicate that if has received broadcast ESPNOW data or not.
    uint8_t id;
    uint32_t resend;                       //
    uint8_t *data;                      //Buffer pointing to ESPNOW data.
    int len;                              //Length of ESPNOW data to be sent, unit: byte.
    uint8_t target_mac[6];   //MAC address of target device.
} espnow_send_packet_t;

/*------------------------------Exertn----------------------------------*/
void bsp_now_init(void);

#endif // !BSP_NOW_H__
