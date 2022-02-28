/*
 * @Description: When esp_now is turned on, the wifi cannot be connected because the two cannot be used at the same time
 * @Author: YZ
 * @Date: 2022-02-28 09
 * @Path: D:\ESP32\template\components\bsp_now\bsp_now.c
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "bsp_now.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_crc.h"
#include "esp_log.h"

static uint8_t target_addr[ESP_NOW_ETH_ALEN] = {0xc4, 0x5b, 0xbe, 0x3f, 0x5b, 0x44};

static QueueHandle_t espnow_send_queue;
static QueueHandle_t espnow_recv_queue;
static espnow_send_packet_t send_packet;
static espnow_recv_packet_t recv_packet;

static const char *TAG = "espnow";
static uint8_t test[] = "hello";

/**
* @description: ESPNOW sending callback function is called in WiFi task.
* @param:  null
* @return: null
* @note: not do lengthy operations from this task,post necessary data to a queue and handle it from a lower priority task.
*/
static void espnow_send_callback( const uint8_t *mac_addr, esp_now_send_status_t status )
{
    if( mac_addr == NULL )
    {
        ESP_LOGE( TAG, "Send cb arg error" );
        return;
    }
    memcpy( send_packet.target_mac, mac_addr, ESP_NOW_ETH_ALEN );
    send_packet.status = status;
    if( xQueueSend( espnow_send_queue, &send_packet, 0 ) != pdTRUE )
    {
        ESP_LOGW( TAG, "Send send queue fail" );
    }
}

/**
* @description: ESPNOW receiving callback function is called in WiFi task.
* @param:  null
* @return: null
* @note:
*/
static void espnow_recv_callback( const uint8_t *mac_addr, const uint8_t *data, int len )
{
    if( mac_addr == NULL || data == NULL || len <= 0 )
    {
        ESP_LOGE( TAG, "Receive cb arg error" );
        return;
    }
    memcpy( &recv_packet.sender_mac, mac_addr, ESP_NOW_ETH_ALEN );
    memcpy( &recv_packet.data, data, len);
    recv_packet.len = len;
    if( xQueueSend( espnow_recv_queue, &recv_packet, 0 ) != pdTRUE )
    {
        ESP_LOGW( TAG, "Send receive queue fail" );
    }
}

/**
* @description: Parse received ESPNOW data.
* @param:  null
* @return: null
* @note:
*/
static int32_t  espnow_data_parse( void )
{
    // uint16_t crc_recv, crc_math = 0;

    // if( len < sizeof( espnow_recv_packet_t ) )
    // {
    //     ESP_LOGE( TAG, "Receive ESPNOW data too short, len:%d", data_len );
    //     return -1;
    // }
    // crc_recv = buf->crc;
    // crc_math = esp_crc16_le( UINT16_MAX, ( uint8_t const * )buf, data_len );
    // if( crc_math == crc_recv )
    // {
        
    // }
    return -1;
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
static void espnow_data_prepare( void )
{
    // uint16_t rcr_math = 0;

    // rcr_math = esp_crc16_le( UINT16_MAX, ( uint8_t const * )send_packet.data, send_packet.len );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
static void espnow_send_task( void *arg )
{
    ESP_LOGI( TAG, "Start espnow send task." );
    
    espnow_send_packet_t packte;
    memset(&packte, 0, sizeof( espnow_send_packet_t));

    while( 1 )
    {
        if(xQueueReceive( espnow_send_queue, &packte, portMAX_DELAY ) == pdTRUE)
        {
            ESP_LOGI( TAG, "Send data to "MACSTR", status1: %d", MAC2STR(packte.target_mac), packte.status);

            if (memcpy(packte.target_mac, target_addr, ESP_NOW_ETH_ALEN) != 0) 
            {
                packte.id = 1;
                if (packte.id != 0) 
                {
                    // packte.id--;
                    // espnow_data_prepare();
                    /* Send the next data after the previous data is sent. */
                    if (esp_now_send(target_addr, test, sizeof(test)) != ESP_OK) 
                    {
                        //TODO rerty send data
                    }
                    
                }
            }
        }
        vTaskDelay( pdMS_TO_TICKS(1000));
    }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
static void espnow_recv_task(void *arg)
{
    ESP_LOGI( TAG, "Start espnow recive task." );

    espnow_recv_packet_t packte;
    memset(&packte, 0, sizeof( espnow_recv_packet_t));
    
    while( 1 )
    {
        if (xQueueReceive( espnow_recv_queue, &packte, portMAX_DELAY ) == pdTRUE) 
        {
            ESP_LOGI(TAG, "Receive broadcast data from: "MACSTR", len: %d", MAC2STR(packte.sender_mac), packte.len);
            // espnow_data_parse();
            // /* If MAC address does not exist in peer list, add it to peer list. */
            // if (esp_now_is_peer_exist(packte.sender_mac) == false) 
            // {
            //     esp_now_peer_info_t peer_info;
            //     memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
            //     memcpy(peer_info.peer_addr, packte.sender_mac, ESP_NOW_ETH_ALEN);
            //     memcpy(peer_info.lmk, BSP_ESPNOW_LMK, ESP_NOW_KEY_LEN);
            //     peer_info.channel = BSP_ESPNOW_CHANNEL;
            //     peer_info.ifidx = BSP_ESPNOW_WIFI_IF;
            //     peer_info.encrypt = true;
            //     esp_now_add_peer(&peer_info);
            // }
        }
    }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_now_send_data(uint8_t *data, uint8_t len)
{
    
}

 /**
 * @description: Must init nvs and wifi before init esp_now.
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_now_init(void)
{
    /* Initialize ESPNOW and register sending and receiving callback function. */
    ESP_ERROR_CHECK( esp_now_init() );
    ESP_ERROR_CHECK( esp_now_register_send_cb(espnow_send_callback) );
    ESP_ERROR_CHECK( esp_now_register_recv_cb(espnow_recv_callback) );

    /* Set primary master key. */
    ESP_ERROR_CHECK( esp_now_set_pmk((const uint8_t *)BSP_ESPNOW_PMK) );

    /* Add broadcast peer information to peer list. */
    esp_now_peer_info_t peer_info;
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
    memcpy(peer_info.peer_addr,target_addr, ESP_NOW_ETH_ALEN);
    // memcpy(peer_info.lmk, BSP_ESPNOW_LMK, ESP_NOW_KEY_LEN);
    peer_info.channel = BSP_ESPNOW_CHANNEL;
    peer_info.ifidx = BSP_ESPNOW_WIFI_IF;
    peer_info.encrypt = false;
    esp_now_add_peer(&peer_info);

#if (BSP_NOW_MASTER)
    espnow_recv_queue = xQueueCreate(BSP_ESPNOW_QUEUE_SIZE, sizeof(espnow_recv_packet_t));
    xTaskCreate(espnow_recv_task, "espnow receive task", 2048, NULL, 4, NULL);
#else
    espnow_send_queue = xQueueCreate(BSP_ESPNOW_QUEUE_SIZE, sizeof(espnow_send_packet_t));
    xTaskCreate(espnow_send_task, "espnow send task", 2048, NULL, 4, NULL);
    if( esp_now_send(target_addr, test, sizeof(test)) != ESP_OK )
    {
        ESP_LOGE(TAG, "Send error");
    }
#endif
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_now_deinit(void)
{
    vSemaphoreDelete(espnow_send_queue);
    vSemaphoreDelete(espnow_recv_queue);
    vTaskDelete(&espnow_send_task);
    vTaskDelete(&espnow_recv_task);
    esp_now_deinit();
}