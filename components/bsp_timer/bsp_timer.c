/*
 * @Description: 
 * @Author: YZ
 * @Date: 2022-01-21 11
 * @Path: D:\ESP32\template\components\bsp_timer\bsp_timer.c
 */
#include <stdio.h>
#include "bsp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/timer.h"
#include "esp_log.h"

// static const char *TAG = "timer";

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
static bool bsp_timer_callback(void *arg)
{
    return pdFALSE;
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_timer_init(void)
{
    timer_user_data_t timer_data = {
        .timer_group = 0,
        .timer_idx = 0,
        .auto_reload = 0,
        .alarm_value = 521
    };
    timer_config_t config = {
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = 1024,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .divider = (APB_CLK_FREQ / BSP_TIMER_FREQUENCY ),
        .intr_type = TIMER_INTR_LEVEL
    };
    timer_init(timer_data.timer_group,timer_data.timer_idx,&config);
    timer_set_alarm_value(timer_data.timer_group,timer_data.timer_idx,timer_data.alarm_value);
    timer_enable_intr(timer_data.timer_group,timer_data.timer_idx);
    timer_isr_callback_add(timer_data.timer_group,timer_data.timer_idx,bsp_timer_callback,"timer",0);
    timer_start(timer_data.timer_group,timer_data.timer_idx);
}
