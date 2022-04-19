#ifndef __BSP_MI2C_H__
#define __BSP_MI2C_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include <stdint.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
typedef struct
{
    uint8_t *name;
    uint8_t port;
    uint8_t address;
} bsp_i2c_dev_t;

/*------------------------------Exertn----------------------------------*/
void bsp_mi2c_init(uint8_t port, uint32_t scl, uint32_t sda, uint32_t frequency);
void bsp_mi2c_write(bsp_i2c_dev_t *dev, uint8_t *data, size_t size);
void bsp_mi2c_read(bsp_i2c_dev_t *dev, uint8_t *data, size_t size);
void bsp_mi2c_write_reg(bsp_i2c_dev_t *dev, uint8_t reg, uint8_t *data, size_t size);
void bsp_mi2c_read_reg(bsp_i2c_dev_t *dev, uint8_t reg, uint8_t *data, size_t size);
void bsp_mi2c_read_16bit_reg(bsp_i2c_dev_t *dev, uint16_t reg, uint8_t *data, size_t size);
#endif // !BSP_MI2C_H__
