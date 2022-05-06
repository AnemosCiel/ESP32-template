/*
 * @Description: bsp_server
 * @Author: 
 * @Date: 2022-04-27 16
 * @Path: D:\ESP32\template\components\bsp_wifi\bsp_server.c
 */
#include "bsp_wifi.h"
#include <stdio.h>
#include <string.h>
// #include <sys/param.h>
// #include <sys/unistd.h>
// #include <sys/stat.h>
#include <dirent.h>

#include "esp_err.h"
#include "esp_log.h"

// #include "esp_vfs.h"
// #include "esp_spiffs.h"
#include "esp_http_server.h"

#include "cJSON.h"

char user_id[100];
char user_code[100];

static const char *TAG = "server";

static esp_err_t index_get_handler(httpd_req_t *req)
{
    /* Send back a simple response packet */
    extern const unsigned char index_start[] asm("_binary_index_html_start");
    extern const unsigned char index_end[]   asm("_binary_index_html_end");
    const size_t index_size = (index_end - index_start);
    ESP_LOGI(TAG, "send html");
    httpd_resp_send(req, (const char *)index_start, index_size);
    return ESP_OK;
}

static esp_err_t send_wifi_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = req->user_ctx;
    int received = 0;
    
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Check for timeout */
            if (received == HTTPD_SOCK_ERR_TIMEOUT) {
                /* If it is a timeout, you can call httpd_req_recv() to retry
                * For simplicity, here we directly respond to the HTTP 408 (request timed out) error to the client */
                httpd_resp_send_408(req);
            }
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    
    buf[total_len] = '\0';
    printf("recived data length is :%d\n",total_len);
    for (int i = 0; i <total_len ; i++){
        putchar(buf[i]);
    }
    printf("\r\nwifi data recived!\r\n");

    cJSON *root = cJSON_Parse(buf);
    char *ssid = cJSON_GetObjectItem(root, "wifi_name")->valuestring;
    char *code = cJSON_GetObjectItem(root, "wifi_code")->valuestring;
    int len1 = strlen(ssid);
    int len2 = strlen(code);
    memcpy(user_id,ssid,strlen(ssid));
    memcpy(user_code,code,strlen(code));
    user_id[len1] = '\0';
    user_code[len2] = '\0';
    cJSON_Delete(root);
    // ESP_LOGI(TAG, "SSID:%d password:%s ",ssid,code);
    // ESP_LOGI(TAG, "SSID:%s password:%s ",user_id,user_code);
    printf("\r\nwifi_ssid:");
    for(int i = 0;i<len1;i++){
        printf("%c",user_id[i]);
    }

    printf("\r\nwifi_code:");
    for(int i = 0;i<len2;i++){
        printf("%c",user_code[i]);
    }
    printf("\r\n");
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

 /**
 * @description: The function that starts the web server
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_server_start(void)
{
    /* The instance handle of esp_http_server*/
    httpd_handle_t server = NULL;
    /* Generate default configuration parameters */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG(); 

    /* Use the URI wildcard matching function in order to
     * allow the same handler to respond to multiple different
     * target URIs which match the wildcard scheme */
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(TAG, "Starting HTTP Server");
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start file server!");
        return;
    }

    /* URI handler for getting uploaded files */
    httpd_uri_t uri_get = {
        .uri       = "/", 
        .method    = HTTP_GET,
        .handler   = index_get_handler,
        .user_ctx  = NULL    // Pass server data as context
    };
    httpd_register_uri_handler(server, &uri_get);

    /* URI handler for uploading files to server */
    httpd_uri_t wifi_data = {
        .uri       = "/",   // Match all URIs of type /upload/path/to/file
        .method    = HTTP_POST,
        .handler   = send_wifi_handler,
        .user_ctx  = NULL    // Pass server data as context
    };
    httpd_register_uri_handler(server, &wifi_data);
}

/**
 * @description: Function to stop the web server
 * @param:  null
 * @return: null
 * @note: 
 */
void stop_webserver(httpd_handle_t server)
{
    if (server) {
        /* stop httpd server */
        httpd_stop(server);
    }
}


