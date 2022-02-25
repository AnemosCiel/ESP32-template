/*
 * @Description: 
 * @Author: YZ
 * @Date: 2022-02-25 10
 * @Path: D:\ESP32\template\components\bsp_mcpwm\bsp_mcpwm.c
 */
#include <stdio.h>
#include "bsp_mcpwm.h"
#include "driver/mcpwm.h"

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_mcpwm_register(uint8_t unit, uint8_t channel, uint8_t pin)
{
    mcpwm_gpio_init(unit, channel, pin); // To drive a RC servo, one MCPWM generator is enough

    mcpwm_config_t pwm_config = {
        .frequency = 10000, // frequency = 50Hz
        .cmpr_a = 0,     // duty cycle of PWMxA = 0
        .counter_mode = MCPWM_UP_COUNTER,   // Up counter
        .duty_mode = MCPWM_DUTY_MODE_0,     // Active HIGH
    };
    mcpwm_init(unit, MCPWM_TIMER_0, &pwm_config);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_mcpwm_registerAB(uint8_t unit, uint8_t pinA, uint8_t pinB)
{
    mcpwm_gpio_init(unit, (unit*2  ), pinA);
    mcpwm_gpio_init(unit, (unit*2+1), pinB);

    mcpwm_config_t pwm_config = {
        .frequency = 10000, // frequency = 50Hz
        .cmpr_a = 0,     // duty cycle of PWMxA = 0
        .cmpr_b = 0,
        .counter_mode = MCPWM_UP_DOWN_COUNTER,   // Up-Down counter
        .duty_mode = MCPWM_DUTY_MODE_0,     // Active HIGH
    };
    mcpwm_init(unit, unit, &pwm_config);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_mcpwm_set_duty(uint8_t unit, uint8_t channel, uint8_t duty)
{
    mcpwm_set_duty(unit, unit, channel, duty);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_mcpwm_set_dutyAB(uint8_t unit, uint8_t dutyA, uint8_t dutyB)
{
    mcpwm_set_duty(unit, unit, (unit*2  ), dutyA);
    mcpwm_set_duty(unit, unit, (unit*2+1), dutyB);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_mcpwm_start(uint8_t unit)
{
    mcpwm_start(unit,unit);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_mcpwm_stop(uint8_t unit)
{
    mcpwm_stop(unit,unit);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_mcpwm_init(void)
{
    bsp_mcpwm_register(MCPWM_UNIT_0, MCPWM0A, 18);
    bsp_mcpwm_register(MCPWM_UNIT_0, MCPWM0B, 19);
    bsp_mcpwm_set_duty(MCPWM_UNIT_0, MCPWM0A, 50);
    bsp_mcpwm_set_duty(MCPWM_UNIT_0, MCPWM0B, 50);
    bsp_mcpwm_start(MCPWM_UNIT_0);
}