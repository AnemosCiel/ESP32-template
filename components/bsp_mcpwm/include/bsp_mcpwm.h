#ifndef __BSP_MCPWM_H__
#define __BSP_MCPWM_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/

/*------------------------------Exertn----------------------------------*/
void bsp_mcpwm_register(uint8_t unit, uint8_t channel, uint8_t pin);
void bsp_mcpwm_registerAB(uint8_t uint, uint8_t pinA, uint8_t pinB);

void bsp_mcpwm_set_duty(uint8_t unit, uint8_t channel, uint8_t duty);
void bsp_mcpwm_set_dutyAB(uint8_t unit, uint8_t dutyA, uint8_t dutyB);

void bsp_mcpwm_start(uint8_t unit);
void bsp_mcpwm_stop(uint8_t unit);

void bsp_mcpwm_init(void);

#endif // !BSP_MCPWM_H__
