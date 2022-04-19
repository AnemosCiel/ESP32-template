#ifndef __BSP_MI2C_H__
#define __BSP_MI2C_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/

#define BSP_MI2C_ACK_EN     1
#define BSP_MI2C_ACK_DISEN  0

#define BSP_MI2C_WRITE      0
#define BSP_MI2C_READ       1
/*------------------------------Exertn----------------------------------*/
void bsp_mi2c_init( uint32_t scl, uint32_t sda, uint32_t frequency );
void bsp_mi2c_write( uint8_t dev_addr, uint8_t *data, size_t size );
void bsp_mi2c_read( uint8_t dev_addr, uint8_t *data, size_t size );
void bsp_mi2c_write_reg( uint8_t dev_addr, uint8_t reg, uint8_t *data, size_t size );
void bsp_mi2c_read_reg( uint8_t dev_addr, uint8_t reg, uint8_t *data, size_t size );
void bsp_mi2c_read_16bit_reg( uint8_t dev_addr, uint16_t reg, uint8_t *data, size_t size );
#endif // !BSP_MI2C_H__
