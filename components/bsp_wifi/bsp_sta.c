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

#define MAX_RETRY_CONN_NUM 3
typedef enum
{
    STA_CONN_IDLE,
    STA_CONN_SUCCESS,
    STA_CONN_FAIL,
}STA_CONN_STATE;

static uint8_t retry_conn_cnt = 0;
static uint8_t conn_state = STA_CONN_IDLE;

static const char *TAG = "Wi-Fi STA";

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (retry_conn_cnt < MAX_RETRY_CONN_NUM)
        {
            esp_wifi_connect();
            retry_conn_cnt++;
            ESP_LOGI(TAG, "retry to connect to the AP:%d", retry_conn_cnt);
        }
        else
        {
            conn_state = STA_CONN_FAIL;
            ESP_LOGI(TAG, "connect to the AP fail");
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        conn_state = STA_CONN_SUCCESS;
        retry_conn_cnt = 0;
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

/**
 * @description: Initialize wifi sta mode
 * @param:  ssid: wifi name
 *          password: wifi pass word
 * @return: null
 * @note: ssid length no more than 32 bytes, password length no more than 64 bytes
 */
void bsp_sta_init(uint8_t *ssid, uint8_t *password)
{
    uint8_t ssid_size = strlen((char *)ssid);
    uint8_t password_size = strlen((char *)password);
    if ((ssid_size == 0) || (ssid_size > 32) || (password_size > 64))
    {
        ESP_LOGI(TAG, "ssid length no more than 32 bytes, password length no more than 64 bytes");
        return;
    }
    conn_state = STA_CONN_IDLE;
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            // .ssid = ,
            // .password = ,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };
    strcpy((char *)wifi_config.sta.ssid, (char *)ssid);
    strcpy((char *)wifi_config.sta.password, (char *)password);
    if (password_size == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    while (conn_state != STA_CONN_IDLE)
    {
        ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
        ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    }
}
