/*
 * @Description:
 * @Author:
 * @Date: 2021-12-03 16
 * @Path: D:\ESP32\template\components\src\bsp_wifi.c
 */
#include "bsp_wifi.h"
/*----------driver---------*/
/* log */
#include "esp_log.h"
#include "esp_event.h"
/* wifi */
#include "esp_wifi.h"
/* nvs */
#include "nvs_flash.h"
/* uart */
#include "driver/uart.h"
#include "string.h"
/* bsp */
#include "user.h"

static const char *TAG = "bsp_wifi";
bsp_wifi_t wifi_info[BSP_WIFI_SCAN_LIST_SIZE] = {0};
bsp_whitelist_t white_list[BSP_WIFI_WHITE_LIST_SIZE] = {0};
uint16_t wifi_number; // Calculates the number of scanned AP
SemaphoreHandle_t wifiSemphr = NULL;

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_wifi_whitelist_add( uint8_t id, uint8_t *mac )
{
    memcpy( &white_list[id].mac, mac, BSP_WIFI_MAC_SIZE );
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_wifi_whitelist_init( void )
{
}


/**
 * @description:
 * @param:  null
 * @return: null
 * @note: ID starts from 1
 */
static uint8_t bsp_wifi_whitelist_check( uint8_t *object, bsp_whitelist_t *list )
{
    for( uint8_t i = 0; i < BSP_WIFI_WHITE_LIST_SIZE; i++ )
    {
        if( memcmp( object, list[i].mac, BSP_WIFI_MAC_SIZE ) == 0 )
        {
#if WIFI_INFO
            ESP_LOGI( TAG, "id:%d", i + 1 );
#endif
            return i + 1;
        }
    }
    return 0;
}

/**
 * @description: Check if Wi-Fi exists
 * @param:  null
 * @return: null
 * @note:
 */
uint8_t bsp_wifi_find( uint8_t *mac )
{
    for( uint8_t i = 0; i < BSP_WIFI_SCAN_LIST_SIZE; i++ )
    {
        if( memcmp( mac, &wifi_info[i].mac, BSP_WIFI_MAC_SIZE ) == 0 )
        {
            return i; // Return index
        }
    }
    return -1;
}

/**
 * @description: Add Wi-Fi
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_wifi_add( uint8_t index, uint8_t id, wifi_ap_record_t *ap )
{
    memcpy( wifi_info[index].mac, ap->bssid, BSP_WIFI_MAC_SIZE );
    wifi_info[index].rssi = ap->rssi;
    wifi_info[index].id = id;
#if WIFI_INFO
    ESP_LOGI( TAG, "wifi_info[%d]  id:%d  rssi:%4d  mac:%02x:%02x:%02x:%02x:%02x:%02x\n", index, id, wifi_info[id].rssi, wifi_info[index].mac[0],
              wifi_info[index].mac[1],
              wifi_info[index].mac[2],
              wifi_info[index].mac[3],
              wifi_info[index].mac[4],
              wifi_info[index].mac[5] );
#endif
}

/**
 * @description: Printf Wi-Fi information
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_wifi_print( void )
{
    ESP_LOGI( TAG, "Scan %d wifi", wifi_number );
    ESP_LOGI( TAG, "===================================================================\n" );
    ESP_LOGI( TAG, "          ID          |    RSSI    |               MAC             \n" );
    ESP_LOGI( TAG, "===================================================================\n" );
    for( uint8_t i = 0; i < wifi_number; i++ )
    {
        ESP_LOGI( TAG, "%d    |    %4d    |    %02x:%02x:%02x:%02x:%02x:%02x\n", i, wifi_info[i].rssi, wifi_info[i].mac[0],
                  wifi_info[i].mac[1],
                  wifi_info[i].mac[2],
                  wifi_info[i].mac[3],
                  wifi_info[i].mac[4],
                  wifi_info[i].mac[5] );
    }
}

/**
 * @description: Wi-Fi search
 * @param:  null
 * @return: null
 * @note:
 */
static void bsp_wifi_scan( void )
{
    uint16_t apCount = 0;
    uint8_t id = 0;
    uint8_t index = 0;
    esp_wifi_scan_start( NULL, true ); /* Default scan mode, block until the scan is complete */
    esp_wifi_scan_get_ap_num( &apCount ); // Get the number of scanned APs
#if WIFI_INFO
    ESP_LOGI( TAG, "Number of access points found: %d\n", apCount );
#endif
    if( apCount == 0 )
    {
        return;
    }
    wifi_ap_record_t *ap_info = ( wifi_ap_record_t * )malloc( sizeof( wifi_ap_record_t ) * apCount );
    wifi_number = ( apCount > BSP_WIFI_SCAN_LIST_SIZE ) ? BSP_WIFI_SCAN_LIST_SIZE : apCount;
    ESP_ERROR_CHECK( esp_wifi_scan_get_ap_records( &wifi_number, ap_info ) );
#if WIFI_INFO
    ESP_LOGI( TAG, "======================================================================\n" );
    ESP_LOGI( TAG, "             SSID             |    RSSI    |           MAC           \n" );
    ESP_LOGI( TAG, "======================================================================\n" );
#endif
    for( uint8_t i = 0; i < wifi_number; i++ )
    {
#if WIFI_INFO
        ESP_LOGI( TAG, "%26.26s    |    %4d    |    %02x:%02x:%02x:%02x:%02x:%02x\n", ap_info[i].ssid, ap_info[i].rssi, ap_info[i].bssid[0],
                  ap_info[i].bssid[1],
                  ap_info[i].bssid[2],
                  ap_info[i].bssid[3],
                  ap_info[i].bssid[4],
                  ap_info[i].bssid[5] );
#endif
        /* If the AP is in the whitelist, get the ID */
        id = bsp_wifi_whitelist_check( ap_info[i].bssid, white_list );
        if( id != 0 )
        {
            bsp_wifi_add( index, id, &ap_info[i] );
            index++;
        }
    }
    xSemaphoreGive( wifiSemphr );
    free( ap_info );
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_wifi_task( void *arg )
{
    while( 1 )
    {
        bsp_wifi_scan();
        vTaskDelay( pdMS_TO_TICKS( 5000 ) );
    }
}

/**
 * @description: Wi-Fi initialization
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_wifi_init( void )
{
    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if( ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND )
    {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    ESP_ERROR_CHECK( esp_netif_init() );
    ESP_ERROR_CHECK( esp_event_loop_create_default() );
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init( &cfg ) );
    ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    bsp_wifi_whitelist_init();
    wifiSemphr = xSemaphoreCreateBinary();
    xTaskCreate( bsp_wifi_task, "wifi_task", 1024 * 4, NULL, 12, NULL );
}