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
/*------------------------------Exertn----------------------------------*/
void bsp_now_init(void);

#endif // !BSP_NOW_H__
