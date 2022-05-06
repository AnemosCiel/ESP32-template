/*
 * @Description: bsp_ap
 * @Author:
 * @Date: 2022-04-27 14
 * @Path: D:\ESP32\template\components\bsp_wifi\bsp_ap.c
 */
#include "bsp_wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "Wi-Fi AP";

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

/**
 * @description: Initialize wifi ap mode
 * @param:  ssid: wifi name
 *          password: wifi pass word
 *          channel: wifi channel
 *          conn_limit: Maximum number of connections
 * @return: null
 * @note: ssid length no more than 32 bytes, password length no more than 64 bytes
 */
void bsp_ap_init(uint8_t *ssid, uint8_t *password, uint8_t channel, uint8_t conn_limit)
{
    uint8_t ssid_size = strlen((char *)ssid);
    uint8_t password_size = strlen((char *)password);
    if ((ssid_size == 0) || (ssid_size > 32) || (password_size > 64))
    {
        ESP_LOGI(TAG, "ssid length no more than 32 bytes, password length no more than 64 bytes");
        return;
    }

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            // .ssid = ssid,
            .ssid_len = strlen((char *)ssid),
            .channel = channel,
            // .password = password,
            .max_connection = conn_limit,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    strcpy((char *)wifi_config.ap.ssid, (char *)ssid);
    strcpy((char *)wifi_config.ap.password, (char *)password);
    if (password_size == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             ssid, password, channel);
}
