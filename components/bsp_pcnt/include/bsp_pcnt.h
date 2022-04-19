#ifndef __BSP_PCNT_H__
#define __BSP_PCNT_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include <stdint.h>
#include <stdbool.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/

/*------------------------------Exertn----------------------------------*/
int64_t bsp_pcnt_read(uint8_t unit);
int64_t bsp_pcnt_get_lastcount(uint8_t unit);

void bsp_pcnt_clear(uint8_t unit);
void bsp_pcnt_start(uint8_t unit);
void bsp_pcnt_stop(uint8_t unit);

void bsp_pcnt_x1_init(uint8_t unit, uint8_t pin_a, uint8_t pin_b);
void bsp_pcnt_x2_init(uint8_t unit, uint8_t pin_a, uint8_t pin_b);
void bsp_pcnt_x4_init(uint8_t unit, uint8_t pin_a, uint8_t pin_b);

#endif // !BSP_PCNT_H__
