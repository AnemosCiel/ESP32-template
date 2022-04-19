#ifndef __BSP_LEDC_H__
#define __BSP_LEDC_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
#include "driver/ledc.h"
/*------------------------------Define----------------------------------*/

/*------------------------------Exertn----------------------------------*/
void bsp_ledc_set_freq(uint8_t mode, uint8_t timer, uint32_t frequency);
void bsp_ledc_set_duty(uint8_t mode, uint8_t channel, uint8_t duty);
void bsp_ledc_set_fade(uint8_t mode, uint8_t channel, uint32_t duty, uint32_t time);
void bsp_ledc_start(uint8_t mode, uint8_t channel);
void bsp_ledc_stop(uint8_t mode, uint8_t channel, uint8_t level);
void bsp_ledc_ls_register(uint8_t gpio, uint8_t channel, uint8_t timer, uint32_t freq);
void bsp_ledc_hs_register(uint8_t gpio, uint8_t channel, uint8_t timer, uint32_t freq);
#endif // !BSP_LEDC_H__
