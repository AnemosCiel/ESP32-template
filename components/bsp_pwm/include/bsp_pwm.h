#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
#include "driver/ledc.h"
/*------------------------------Define----------------------------------*/
#define BSP_PWM_MODE    LEDC_LOW_SPEED_MODE // High or low speed mode
#define BSP_PWM_TIMER   LEDC_TIMER_0 // Timer0~3
#define BSP_PWM_RESOLUTION LEDC_TIMER_13_BIT
#define BSP_PWM_DEFAULT_FREQ    5000
/*------------------------------Exertn----------------------------------*/
void bsp_pwm_start( uint8_t channel );
void bsp_pwm_stop( uint8_t channel, uint8_t level );
void bsp_pwm_set_freq( uint32_t frequency );
void bsp_pwm_set_duty( uint8_t channel, uint8_t duty );
void bsp_pwm_register( uint8_t gpio, uint8_t channel );
void bsp_pwm_init( void );
#endif // !BSP_PWM_H__
