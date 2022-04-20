/*
 * @Description: app_led
 * @Author: 
 * @Date: 2022-04-20 11
 * @Path: D:\ESP32\template\components\app_led\app_led.c
 */
#include <stdio.h>
#include <string.h>
#include "app_led.h"
#include "bsp_gpio.h"

app_led_t hled[APP_LED_HANDLE_MAX];

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void app_led_set_time(uint8_t id, uint32_t ontime, uint32_t offtime)
{
    hled[id].on_time = ontime;
    hled[id].off_time = offtime;
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void app_led_list_id(void)
{
    printf("led used id list:");
    for(uint8_t i = 0; i < APP_LED_HANDLE_MAX; i++)
    {
        if(hled[i].valid != 0)
        {
            printf("%d",i);
        }
    }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void app_led_delete(uint8_t id)
{
    if(id >= APP_LED_HANDLE_MAX)
    {
        return;
    }
    if(hled[id].valid != 0)
    {
        memset(&hled[id], 0, sizeof(app_led_t));
    }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void app_led_register(uint8_t id, uint8_t pin, uint8_t level, APP_LED_MODE mode)
{
    if(id >= APP_LED_HANDLE_MAX)
    {
        return;
    }
    if(hled[id].valid == 0)
    {
        hled[id].id = id;
        hled[id].pin = pin;

        hled[id].level = level;
        hled[id].mode = mode;
        hled[id].valid = 1;
        bsp_gpio_inout(pin);
        bsp_gpio_write(pin, !level);
    }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void app_led_core(void)
{
    uint8_t level = 0;

    for(uint8_t i = 0; i < APP_LED_HANDLE_MAX; i++)
    {
        if(hled[i].valid != 0)
        {
            level = bsp_gpio_read(hled[i].pin);
            hled[i].counter++;

            if(hled[i].mode == APP_LED_SINGLE)
            {
                if(level == hled[i].level) // on
                {
                    if(hled[i].counter >= hled[i].on_time)
                    {
                        hled[i].counter = 0;
                        hled[i].on_time = 0;
                        bsp_gpio_write(hled[i].pin, !hled[i].level);
                    }
                }
                else // off
                {
                    if(hled[i].counter >= hled[i].off_time)
                    {
                        hled[i].counter = 0;
                        hled[i].off_time = 0;
                        bsp_gpio_write(hled[i].pin, hled[i].level);
                    }
                }
                if((!hled[i].on_time) && (!hled[i].off_time))
                {
                    bsp_gpio_reset(hled[i].pin);
                    app_led_delete(i);
                }
            }
            else
            {
                if(level == hled[i].level) // on
                {
                    if(hled[i].counter >= hled[i].on_time)
                    {
                        hled[i].counter = 0;
                        bsp_gpio_write(hled[i].pin, !hled[i].level);
                    }
                }
                else // off
                {
                    if(hled[i].counter >= hled[i].off_time)
                    {
                        hled[i].counter = 0;
                        bsp_gpio_write(hled[i].pin, hled[i].level);
                    }
                }
            }
        }
    }
}

