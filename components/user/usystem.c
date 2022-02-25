/*
 * @Description: 
 * @Author: YZ
 * @Date: 2022-01-22 15
 * @Path: D:\ESP32\template\components\user\system.c
 */
#include "usystem.h"
#include "esp_system.h"
#include "esp32/rom/ets_sys.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char *TAG = "usystem";

/**
 * @description: get system tick since boot start
 * @param:  null
 * @return: system tick
 * @note:
 */
int64_t user_get_systemtick( void )
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGD( TAG, "Periodic timer called, time since boot: %lld us", time_since_boot );
    return time_since_boot;
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
int32_t user_get_runtime( int64_t start )
{
    int64_t stop = user_get_systemtick();
    int64_t time = stop - start;
    time = ( time < 0 ) ? ( -time ) + 0xFFFFFFFF / 2 : time;
    ESP_LOGD( TAG, "Run time: %lld us", time );   
    return time;
}