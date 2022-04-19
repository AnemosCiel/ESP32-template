/*
 * @Description:
 * @Author: YZ
 * @Date: 2022-01-22 15
 * @Path: D:\ESP32\template\components\bsp_gpio\bsp_gpio.c
 */
#include <stdio.h>
#include "bsp_gpio.h"
#include "esp32/rom/ets_sys.h"

static gpio_isr_t input_callback = NULL;

/**
 * @description: gpio input callback function
 * @param:  arg：gpio pin number
 * @return: null
 * @note:
 */
static void bsp_gpio_input_handler(void *arg)
{
    if (input_callback != NULL)
    {
        input_callback(arg);
    }
}

/**
 * @description: Initialize gpio as input
 * @param:  gpio_num：gpio pin number
 *          mode：pull mode
 *          type：interrupt type
 *          input_isr_callback：input callback function
 * @return: null
 * @note:
 */
void bsp_gpio_input( uint32_t gpio_num, bsp_iopull_mode_t mode, bsp_ioint_type_t type, bsp_ioisr_t input_isr_callback )
{
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
    switch (mode)
    {
    case FLOATING:
        gpio_set_pull_mode(gpio_num, GPIO_FLOATING);
        break;
    case PULLUP:
        gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY);
        break;
    case PULLDOWN:
        gpio_set_pull_mode(gpio_num, GPIO_PULLDOWN_ONLY);
        break;
    default:
        break;
    }
    switch (type)
    {
    case DISABLE:
        gpio_set_intr_type(gpio_num, GPIO_INTR_DISABLE);
        break;
    case RISING:
        gpio_set_intr_type(gpio_num, GPIO_INTR_POSEDGE);
        break;
    case FALLING:
        gpio_set_intr_type(gpio_num, GPIO_INTR_NEGEDGE);
        break;
    default:
        break;
    }

    if (input_isr_callback != NULL)
    {
        input_callback = input_isr_callback;
        gpio_install_isr_service(0);
        gpio_isr_handler_add(gpio_num, bsp_gpio_input_handler, (void *)gpio_num);
    }
}

/**
 * @description: Initialize gpio as push-pull output
 * @param:  gpio_num：gpio pin number
 * @return: null
 * @note:
 */
void bsp_gpio_outputPP(uint32_t gpio_num)
{
    gpio_pad_select_gpio(gpio_num);
    gpio_reset_pin(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
}

/**
 * @description: Initialize gpio as open-drain output
 * @param:  gpio_num：gpio pin number
 * @return: null
 * @note:
 */
void bsp_gpio_outputOD(uint32_t gpio_num)
{
    gpio_pad_select_gpio(gpio_num);
    gpio_reset_pin(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT_OD);
}

/**
 * @description: Write gpio state
 * @param:  gpio_num：gpio pin number
 *          state: 0: low level  1: high level
 * @return: null
 * @note:
 */
void bsp_gpio_write(uint32_t gpio_num, uint8_t state)
{
    gpio_set_level(gpio_num, state);
}

/**
 * @description: Read gpio state
 * @param:  gpio_num：gpio pin number
 * @return: gpio state
 * @note:
 */
uint8_t bsp_gpio_read(uint32_t gpio_num)
{
    return gpio_get_level(gpio_num);
}

/**
 * @description: Toogle gpio level
 * @param:  gpio_num：gpio pin number
 * @return: null
 * @note:
 */
void bsp_gpio_toogle(uint32_t gpio_num)
{
    uint8_t state = bsp_gpio_read(gpio_num);
    bsp_gpio_write(gpio_num, !state);
}

 /**
 * @description: Reset gpio configuration
 * @param:  gpio_num：gpio pin number
 * @return: null
 * @note: 
 */
void bsp_gpio_reset(uint32_t gpio_num)
{
    gpio_reset_pin(gpio_num);
}

