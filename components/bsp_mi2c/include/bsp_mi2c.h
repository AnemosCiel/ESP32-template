#ifndef __BSP_MI2C_H__
#define __BSP_MI2C_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
#define hi2c 0

#define BSP_MI2C_SCL 6
#define BSP_MI2C_SDA 11

#define BSP_MI2C_FREQ 400000
#define BSP_MI2C_TIMEOUT 1000

#define BSP_MI2C_ACK_EN 1
#define BSP_MI2C_ACK_DISEN 0

#define BSP_MI2C_TXBUF_SIZE 0
#define BSP_MI2C_RXBUF_SIZE 0

#define BSP_MI2C_WRITE 1
#define BSP_MI2C_READ 0
/*------------------------------Exertn----------------------------------*/

#endif // !BSP_MI2C_H__
