/*
 * @Description:
 * @Author:
 * @Date: 2021-12-10 11
 * @Path: D:\ESP32\Pwc\components\bsp_adc\bsp_adc.c
 */
/***********lib***********/
#include <stdio.h>
#include <stdlib.h>
#include "bsp_adc.h"
/* rtos */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/***********driver***********/
/* gpio */
#include "driver/gpio.h"
/* adc */
#include "driver/adc.h"
#include "esp_adc_cal.h"
/* log */
#include "esp_log.h"


 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_adc1_channel_init( uint8_t channel )
{
    if(channel > ADC1_CHANNEL_MAX)
    {
        return;
    }
    adc1_config_width( BSP_ADC_WIDTH );
    adc1_config_channel_atten( channel, BSP_ADC_ATTEN );
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_adc2_channel_init( uint8_t channel )
{
    if(channel > ADC2_CHANNEL_MAX)
    {
        return;
    }
    adc2_config_channel_atten( channel, BSP_ADC_ATTEN );
}

 /**
 * @description: get adc1 channel value
 * @param:  channel：adc channel
 * @return: adc value
 * @note: 
 */
int32_t bsp_adc1_getvalue( uint8_t channel )
{
    int32_t value = 0;

    if(channel > ADC1_CHANNEL_MAX)
    {
        return 0;
    }
    value = adc1_get_raw( channel );
    return value;
}

 /**
 * @description: get adc2 channel value
 * @param:  channel：adc channel
 * @return: adc value
 * @note: 
 */
int32_t bsp_adc2_getvalue( uint8_t channel )
{
    int32_t value = 0;

    if(channel > ADC2_CHANNEL_MAX)
    {
        return 0;
    }
    adc2_get_raw( channel, BSP_ADC_WIDTH, &value );
    return value;
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
int32_t bsp_adc_get_voltage( int32_t adc_value )
{
    return adc_value * 2600 / 4096;
}
