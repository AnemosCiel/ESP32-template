/*
 * @Description:
 * @Author: YZ
 * @Date: 2022-01-22 15
 * @Path: D:\ESP32\template\components\bsp_gpio\bsp_gpio.c
 */
#include <stdio.h>
#include "bsp_gpio.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"
/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_gpio_square_wave( uint32_t pin, uint32_t numbers, uint32_t delay_us )
{
    while( numbers )
    {
        gpio_set_level( pin, 1 );
        ets_delay_us( delay_us );
        gpio_set_level( pin, 0 );
        ets_delay_us( delay_us );
        numbers--;
    }
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_gpio_init( void )
{
    gpio_pad_select_gpio( 22 );
    gpio_reset_pin( 22 );
    gpio_set_direction( 22, GPIO_MODE_INPUT_OUTPUT);
}
