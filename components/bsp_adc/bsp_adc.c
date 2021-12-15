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

static const char *TAG = "bsp_adc";

/**
 * @description: Check that the TP two-point calibration value and the Vref reference voltage value are burned to eFuse
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_adc_task(void *arg)
{
    uint32_t adc_value = 0;
    uint32_t voltage = 0;
    int raw;

    while (1)
    {
        for (uint8_t i = 0; i < BSP_ADC_FILTER_SIZE; i++)
        {
            if (hadc == ADC_UNIT_1)
            {
                raw = adc1_get_raw((adc1_channel_t)BSP_ADC_CHANNEL);
            }
            else
            {
                adc2_get_raw((adc2_channel_t)BSP_ADC_CHANNEL, BSP_ADC_WIDTH, &raw);
            }
            adc_value += raw;
        }
        adc_value /= BSP_ADC_FILTER_SIZE;
        voltage = adc_value * 2600 / 4096;
#if ADC_INFO
        ESP_LOGI(TAG, "ADC:%d,Voltage value:%d mV\n", adc_value, voltage);
#endif
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_adc_init(void)
{
    /* Check if Two Point or Vref are burned into eFuse */

    if (hadc == ADC_UNIT_1)
    {
        adc1_config_width(BSP_ADC_WIDTH);
        adc1_config_channel_atten(BSP_ADC_CHANNEL, BSP_ADC_ATTEN);
    }
    else
    {
        adc2_config_channel_atten((adc2_channel_t)BSP_ADC_CHANNEL, BSP_ADC_ATTEN);
    }

    xTaskCreate(bsp_adc_task, "adc_task", 1024 * 2, NULL, 10, NULL);
}
