/*
 * @Description:
 * @Author:
 * @Date: 2021-12-17 14
 * @Path: D:\ESP32\Pwc\components\bsp_mi2c\bsp_mi2c.c
 */
#include <stdio.h>
#include "bsp_mi2c.h"

#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "mi2c";

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_mi2c_write( uint8_t address, uint8_t *data, size_t size )
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, ( ( address << 1 ) | BSP_MI2C_WRITE ), BSP_MI2C_ACK_EN );
    i2c_master_write( cmd, data, size, BSP_MI2C_ACK_EN );
    i2c_master_stop( cmd );
    i2c_master_cmd_begin( hi2c, cmd, BSP_MI2C_TIMEOUT );
    i2c_cmd_link_delete( cmd );
}

/**
* @description: The mode without writing device registers is applicable to a few I2C devices such as bh1750 and ads1115 / 1118, which usually have few internal registers
* @param:  null
* @return: null
* @note:
*/
void bsp_mi2c_read( uint8_t address, uint8_t *data, size_t size )
{
    if( size == 0 )
    {
        return;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, ( ( address << 1 ) | BSP_MI2C_READ ), BSP_MI2C_ACK_EN );
    if( size > 1 )
    {
        i2c_master_read( cmd, data, size - 1, I2C_MASTER_ACK );
    }
    i2c_master_read_byte( cmd, data + ( size - 1 ), I2C_MASTER_NACK );
    i2c_master_stop( cmd );
    i2c_master_cmd_begin( hi2c, cmd, BSP_MI2C_TIMEOUT );
    i2c_cmd_link_delete( cmd );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_mi2c_write_reg( uint8_t address, uint8_t reg, uint8_t *data, size_t size )
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, ( ( address << 1 ) | BSP_MI2C_WRITE ), BSP_MI2C_ACK_EN );
    i2c_master_write_byte( cmd, reg, BSP_MI2C_ACK_EN );
    i2c_master_write( cmd, data, size, BSP_MI2C_ACK_EN );
    i2c_master_stop( cmd );
    i2c_master_cmd_begin( hi2c, cmd, BSP_MI2C_TIMEOUT );
    i2c_cmd_link_delete( cmd );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_mi2c_read_reg( uint8_t address, uint8_t reg, uint8_t *data, size_t size )
{
    if( size == 0 )
    {
        return;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, ( ( address << 1 ) | BSP_MI2C_WRITE ), BSP_MI2C_ACK_EN );
    i2c_master_write_byte( cmd, reg, BSP_MI2C_ACK_EN );
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, ( ( address << 1 ) | BSP_MI2C_READ ), BSP_MI2C_ACK_EN );
    if( size > 1 )
    {
        i2c_master_read( cmd, data, size - 1, I2C_MASTER_ACK );
    }
    i2c_master_read_byte( cmd, data + ( size - 1 ), I2C_MASTER_NACK );
    i2c_master_stop( cmd );
    i2c_master_cmd_begin( hi2c, cmd, BSP_MI2C_TIMEOUT );
    i2c_cmd_link_delete( cmd );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_mi2c_read_16bit_reg( uint8_t address, uint16_t reg, uint8_t *data, size_t size )
{
    if( size == 0 )
    {
        return;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, ( ( address << 1 ) | BSP_MI2C_WRITE ), BSP_MI2C_ACK_EN );
    i2c_master_write_byte( cmd, ( reg >> 8 ), BSP_MI2C_ACK_EN );
    i2c_master_write_byte( cmd, ( reg & 0xFF ), BSP_MI2C_ACK_EN );
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, ( ( address << 1 ) | BSP_MI2C_READ ), BSP_MI2C_ACK_EN );
    if( size > 1 )
    {
        i2c_master_read( cmd, data, size - 1, I2C_MASTER_ACK );
    }
    i2c_master_read_byte( cmd, data + ( size - 1 ), I2C_MASTER_NACK );
    i2c_master_stop( cmd );
    i2c_master_cmd_begin( hi2c, cmd, BSP_MI2C_TIMEOUT );
    i2c_cmd_link_delete( cmd );
}

/**
* @description:
* @param:  null
* @return: null
* @note:
*/
void bsp_mi2c_init( void )
{
    i2c_config_t config =
    {
        .mode = hi2c,
        .sda_io_num = BSP_MI2C_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = BSP_MI2C_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = BSP_MI2C_FREQ,
    };
    i2c_param_config( hi2c, &config );
    i2c_driver_install( hi2c, config.mode, BSP_MI2C_RXBUF_SIZE, BSP_MI2C_TXBUF_SIZE, 0 );
}
