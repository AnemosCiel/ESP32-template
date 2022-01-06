#ifndef __BSP_MI2C_H__
#define __BSP_MI2C_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
#define hi2c                0

#define BSP_MI2C_SCL        6
#define BSP_MI2C_SDA        11

#define BSP_MI2C_FREQ       400000
#define BSP_MI2C_TIMEOUT    1000

#define BSP_MI2C_ACK_EN     1
#define BSP_MI2C_ACK_DISEN  0

#define BSP_MI2C_TXBUF_SIZE 0
#define BSP_MI2C_RXBUF_SIZE 0

#define BSP_MI2C_WRITE      0
#define BSP_MI2C_READ       1
/*------------------------------Exertn----------------------------------*/
void bsp_mi2c_init( void );
void bsp_mi2c_write( uint8_t address, uint8_t *data, size_t size );
void bsp_mi2c_read( uint8_t address, uint8_t *data, size_t size );
void bsp_mi2c_write_reg( uint8_t address, uint8_t reg, uint8_t *data, size_t size );
void bsp_mi2c_read_reg( uint8_t address, uint8_t reg, uint8_t *data, size_t size );
void bsp_mi2c_read_16bit_reg( uint8_t address, uint16_t reg, uint8_t *data, size_t size );
#endif // !BSP_MI2C_H__
