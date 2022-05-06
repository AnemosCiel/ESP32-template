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

#include "esp_wifi.h"
#include "esp_crc.h"
#include "esp_log.h"

#define BSP_ESPNOW_PMK "pmk1234567890123" // ESPNOW primary master use. The length of ESPNOW primary master must be 16 bytes.
#define BSP_ESPNOW_LMK "lmk1234567890123" // ESPNOW primary master use. The length of ESPNOW local master must be 16 bytes.

#define BSP_ESPNOW_RECV_LEN_MAX (100)

// static QueueHandle_t espnow_send_queue;
static QueueHandle_t espnow_recv_queue;

static uint8_t bsp_now_recv_buff[BSP_ESPNOW_RECV_LEN_MAX];
static uint8_t bsp_now_recv_len;

static const char *TAG = "espnow";

/**
 * @description: ESPNOW sending callback function is called in WiFi task.
 * @param:  null
 * @return: null
 * @note: not do lengthy operations from this task,post necessary data to a queue and handle it from a lower priority task.
 */
static void espnow_send_callback(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    // uint8_t message;

    if (mac_addr == NULL)
    {
        ESP_LOGI(TAG, "Send callback: mac addr is NULL");
        return;
    }

    if (status == ESP_NOW_SEND_SUCCESS)
    {
        // message = ESP_NOW_SEND_SUCCESS;
        // xQueueSend( espnow_send_queue, &message, 0 );
        ESP_LOGI(TAG, "Send Success");
    }
    else
    {
        // message = ESP_NOW_SEND_FAIL;
        // xQueueSend( espnow_send_queue, &message, 0 );
        ESP_LOGI(TAG, "Send Fail");
    }
}

/**
 * @description: ESPNOW receiving callback function is called in WiFi task.
 * @param:  null
 * @return: null
 * @note:
 */
static void espnow_recv_callback(const uint8_t *mac_addr, const uint8_t *data, uint8_t len)
{
    if (mac_addr == NULL || data == NULL || len == 0)
    {
        ESP_LOGI(TAG, "Receive callback args error");
        return;
    }

    memcpy(bsp_now_recv_buff, data, len);
    bsp_now_recv_len = len;
    if (xQueueSend(espnow_recv_queue, &bsp_now_recv_len, 0) != pdTRUE)
    {
        ESP_LOGI(TAG, "Send receive queue fail");
    }
}

/**
 * @description: Write data to fifo
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_now_write(uint8_t *mac_addr, uint8_t id, uint8_t *data, uint8_t len)
{
    if (len == 0)
    {
        return;
    }
    esp_now_send(mac_addr, data, len);
}

/**
 * @description: Read data from list
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_now_read(uint8_t *mac_addr, uint8_t *data, uint8_t len)
{
    if (len == 0)
    {
        return;
    }

    uint8_t datasize = 0;
    if (xQueueReceive(espnow_recv_queue, &datasize, 100))
    {
        memcpy(data, bsp_now_recv_buff, datasize);
    }
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_now_add_peer(wifi_interface_t *interface, const uint8_t *addr, const uint8_t *lmk)
{
    if (esp_now_is_peer_exist(addr))
    {
        ESP_ERROR_CHECK(esp_now_del_peer(addr));
        return;
    }

    esp_now_peer_info_t peer = {0};
    wifi_second_chan_t sec_channel = 0;

    esp_wifi_get_channel(&peer.channel, &sec_channel);
    if (lmk)
    {
        peer.encrypt = true;
        memcpy(peer.lmk, lmk, ESP_NOW_KEY_LEN);
    }
    peer.ifidx = *interface;
    memcpy(peer.peer_addr, addr, ESP_NOW_ETH_ALEN);

    ESP_ERROR_CHECK(esp_now_add_peer(&peer));
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_now_del_peer(const uint8_t *addr)
{
    esp_err_t ret;

    if (esp_now_is_peer_exist(addr))
    {
        ret = esp_now_del_peer(addr);
        ESP_ERROR_CHECK(ret != ESP_OK);
        ESP_LOGI(TAG, "esp_now_del_peer fail, ret: %d", ret);
    }
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
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_callback));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_callback));

    /* Set primary master key. */
    ESP_ERROR_CHECK(esp_now_set_pmk((const uint8_t *)BSP_ESPNOW_PMK));

    // espnow_send_queue = xQueueCreate(6, sizeof(uint8_t));
    espnow_recv_queue = xQueueCreate(6, sizeof(uint8_t));
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_now_deinit(void)
{
    esp_now_unregister_recv_cb();
    esp_now_unregister_send_cb();
    esp_now_deinit();
    // vSemaphoreDelete(espnow_send_queue);
    vSemaphoreDelete(espnow_recv_queue);
}