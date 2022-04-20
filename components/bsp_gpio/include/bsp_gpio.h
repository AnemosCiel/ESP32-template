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
#include "driver/gpio.h"
/*------------------------------Define----------------------------------*/
typedef enum{
    FLOATING,
    PULLUP,
    PULLDOWN,
}bsp_iopull_mode_t;

typedef enum{
    DISABLE,
    RISING,
    FALLING,
}bsp_ioint_type_t;

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef void (*bsp_ioisr_t)(void *);

/*------------------------------Exertn----------------------------------*/
void bsp_gpio_input( uint32_t gpio_num, bsp_iopull_mode_t mode, bsp_ioint_type_t type, bsp_ioisr_t input_isr_callback );
void bsp_gpio_outputPP( uint32_t gpio_num );
void bsp_gpio_outputOD( uint32_t gpio_num );
void bsp_gpio_inout(uint32_t gpio_num);
void bsp_gpio_reset(uint32_t gpio_num);
void bsp_gpio_write(uint32_t gpio_num, uint8_t state);
uint8_t bsp_gpio_read(uint32_t gpio_num);
void bsp_gpio_toogle(uint32_t gpio_num);
#endif // !BSP_GPIO_H__
