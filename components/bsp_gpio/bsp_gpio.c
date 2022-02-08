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
static void bsp_gpio_input_handler( void *arg )
{
    if( input_callback != NULL )
    {
        input_callback( arg );
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
void bsp_gpio_input( uint32_t gpio_num, gpio_pull_mode_t mode, gpio_int_type_t type, gpio_isr_t input_isr_callback )
{
    gpio_pad_select_gpio( gpio_num );
    gpio_set_direction( gpio_num, GPIO_MODE_INPUT );
    gpio_set_pull_mode( gpio_num, mode );
    gpio_set_intr_type( gpio_num, type );
    gpio_install_isr_service( 0 );
    gpio_isr_handler_add( gpio_num, bsp_gpio_input_handler, ( void * )gpio_num );
    if( input_isr_callback != NULL )
    {
        input_callback = input_isr_callback;
    }
}

/**
* @description: Initialize gpio as push-pull output
* @param:  gpio_num：gpio pin number
* @return: null
* @note:
*/
void bsp_gpio_outputPP( uint32_t gpio_num )
{
    gpio_pad_select_gpio( gpio_num );
    gpio_reset_pin( gpio_num );
    gpio_set_direction( gpio_num, GPIO_MODE_OUTPUT );
}

/**
* @description: Initialize gpio as open-drain output
* @param:  gpio_num：gpio pin number
* @return: null
* @note:
*/
void bsp_gpio_outputOD( uint32_t gpio_num )
{
    gpio_pad_select_gpio( gpio_num );
    gpio_reset_pin( gpio_num );
    gpio_set_direction( gpio_num, GPIO_MODE_OUTPUT_OD );
}

/**
* @description: Generates a specified number of pulses
* @param:  gpio_num：gpio pin number
*          numbers：number of pulses
*          delay_us：delay time between pulses
* @return: null
* @note:
*/
void bsp_gpio_square_wave( uint32_t gpio_num, uint32_t numbers, uint32_t delay_us )
{
    while( numbers )
    {
        gpio_set_level( gpio_num, 1 );
        ets_delay_us( delay_us );
        gpio_set_level( gpio_num, 0 );
        ets_delay_us( delay_us );
        numbers--;
    }
}
