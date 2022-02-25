/*
 * @Description:
 * @Author: YZ
 * @Date: 2022-01-06 15
 * @Path: D:\ESP32\template\components\bsp_pwm\bsp_pwm.c
 */
#include <stdio.h>
#include "bsp_pwm.h"
#include "usystem.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_pwm_start( uint8_t channel )
{
    if( channel >= LEDC_CHANNEL_MAX )
    {
        return;
    }
    bsp_pwm_set_duty( channel, ledc_get_duty( BSP_PWM_MODE, channel ) );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_pwm_stop( uint8_t channel, uint8_t level )
{
    if( channel >= LEDC_CHANNEL_MAX )
    {
        return;
    }
    ledc_stop( BSP_PWM_MODE, channel, level );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_pwm_set_freq( uint32_t frequency )
{
    ledc_set_freq( BSP_PWM_MODE, BSP_PWM_TIMER, frequency );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_pwm_set_duty( uint8_t channel, uint8_t duty )
{
    uint8_t tempduty = MAX(duty,100);
    // Set duty
    ESP_ERROR_CHECK( ledc_set_duty( BSP_PWM_MODE, channel, ( tempduty * (1 << BSP_PWM_RESOLUTION) / 100 ) ) );
    // Update duty to apply the new value
    ESP_ERROR_CHECK( ledc_update_duty( BSP_PWM_MODE, channel ) );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_pwm_register( uint8_t gpio, uint8_t channel )
{
    if( channel >= LEDC_CHANNEL_MAX )
    {
        return;
    }
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer =
    {
        .speed_mode       = BSP_PWM_MODE,
        .timer_num        = BSP_PWM_TIMER,
        .duty_resolution  = BSP_PWM_RESOLUTION,
        .freq_hz          = BSP_PWM_DEFAULT_FREQ,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK( ledc_timer_config( &ledc_timer ) );
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel =
    {
        .speed_mode     = BSP_PWM_MODE,
        .channel        = channel,
        .timer_sel      = BSP_PWM_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = gpio,
        .duty           = 0,
        .hpoint         = 0
    };
    ESP_ERROR_CHECK( ledc_channel_config( &ledc_channel ) );
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_pwm_fade(uint8_t channel, uint32_t duty, uint32_t time )
{
    // Initialize fade service.
    ledc_fade_func_install(channel);
    ledc_set_fade_time_and_start(BSP_PWM_MODE, channel, ( duty * (1 << BSP_PWM_RESOLUTION) / 100 ), time, LEDC_FADE_NO_WAIT);
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_pwm_init( void )
{
    /* Example:
    bsp_pwm_register(4, 0);
    bsp_pwm_start(0);
    bsp_pwm_set_duty(0, 20);
    */
    
}
