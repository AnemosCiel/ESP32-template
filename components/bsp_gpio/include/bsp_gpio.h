#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__
/*
*simple way:
*    gpio_pad_select_gpio( pin );
*    gpio_reset_pin( pin );
*    gpio_set_direction( pin, GPIO_MODE_INPUT_OUTPUT);
*    gpio_set_level( pin, level);
*
*struct way:
*    gpio_config_t config =
*    {
*        .intr_type = GPIO_PIN_INTR_DISABLE, // Interrupt disabled 
*        .mode = GPIO_MODE_INPUT_OUTPUT,     // Input an output mode
*        .pin_bit_mask = ( 1ULL << pin ),    // GPIO pin number
*        .pull_down_en = 0,                  // Pull down disable
*        .pull_up_en = 0,                    // Pull up disable
*    };
*    gpio_config( &config );
*/
/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/

/*------------------------------Exertn----------------------------------*/
void bsp_gpio_init( void );
void bsp_gpio_square_wave( uint32_t pin, uint32_t numbers, uint32_t delay_us );
#endif // !BSP_GPIO_H__
