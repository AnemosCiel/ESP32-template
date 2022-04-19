/*
 * @Description:
 * @Author: YZ
 * @Date: 2022-01-06 15
 * @Path: D:\ESP32\template\components\bsp_pwm\bsp_pwm.c
 */
#include <stdio.h>
#include "bsp_ledc.h"
#include "usystem.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_ledc_set_freq(uint8_t mode, uint8_t timer, uint32_t frequency)
{
    ledc_set_freq(mode, timer, frequency);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_ledc_set_duty(uint8_t mode, uint8_t channel, uint8_t duty)
{
    uint8_t tempduty = MAX(duty, 100);
    // Set duty
    ESP_ERROR_CHECK(ledc_set_duty(mode, channel, (tempduty * (1 << LEDC_TIMER_13_BIT) / 100)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(mode, channel));
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_ledc_set_fade(uint8_t mode, uint8_t channel, uint32_t duty, uint32_t time)
{
    // Initialize fade service.
    ledc_fade_func_install(channel);
    ledc_set_fade_time_and_start(mode, channel, (duty * (1 << LEDC_TIMER_13_BIT) / 100), time, LEDC_FADE_NO_WAIT);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_ledc_start(uint8_t mode, uint8_t channel)
{
    if (channel >= LEDC_CHANNEL_MAX)
    {
        return;
    }
    bsp_ledc_set_duty(mode, channel, ledc_get_duty(mode, channel));
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_ledc_stop(uint8_t mode, uint8_t channel, uint8_t level)
{
    if (channel >= LEDC_CHANNEL_MAX)
    {
        return;
    }
    ledc_stop(mode, channel, level);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_ledc_ls_register(uint8_t gpio, uint8_t channel, uint8_t timer, uint32_t freq)
{
    if (channel >= LEDC_CHANNEL_MAX)
    {
        return;
    }
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer =
        {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_num = timer,
            .duty_resolution = LEDC_TIMER_13_BIT,
            .freq_hz = freq,
            .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel =
        {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = channel,
            .timer_sel = timer,
            .intr_type = LEDC_INTR_DISABLE,
            .gpio_num = gpio,
            .duty = 0,
            .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_ledc_hs_register(uint8_t gpio, uint8_t channel, uint8_t timer, uint32_t freq)
{
    if (channel >= LEDC_CHANNEL_MAX)
    {
        return;
    }
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer =
        {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = timer,
            .duty_resolution = LEDC_TIMER_13_BIT,
            .freq_hz = freq,
            .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel =
        {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel = channel,
            .timer_sel = timer,
            .intr_type = LEDC_INTR_DISABLE,
            .gpio_num = gpio,
            .duty = 0,
            .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}
