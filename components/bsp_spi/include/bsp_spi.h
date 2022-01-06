#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
/*  SPI IOMUX(You must use IOMUX if spi speed more then 80MHZ)
    Pin         HSPI            VSPI
    CS0          15              5
    SCLK         14              18
    MISO         12              19
    MOSI         13              23
    QUADWP       2               22
    QUADHD       4               21
*/
#define SPI_INFO   0

#define USER_SPI_HOST SPI2_HOST  /* User only can use SPI2(HSPI) and SPI3(VSPI) */

#define USER_SPI_CS     15
#define USER_SPI_SCLK   14
#define USER_SPI_MISO   12
#define USER_SPI_MOSI   13

#define USER_SPI_QUADWP -1
#define USER_SPI_QUADHD -1

#define USER_SPI_MODE   0
#define USER_SPI_FREQUENCY  SPI_MASTER_FREQ_40M
#define USER_SPI_MAX_TRANSFER_SIZE  (12*(12+1))
#define USER_SPI_QUEUE_SEZE          12
/*------------------------------Exertn----------------------------------*/
void bsp_spi_init( void );
void bsp_spi_transfer( uint8_t *data, uint8_t *rxbuf, uint8_t len );
void bsp_spi_transfer_polling( uint8_t *data, uint8_t *rxbuf, uint8_t len );
void bsp_spi_write_reg( uint8_t reg, uint8_t value );
uint8_t bsp_spi_read_reg( uint8_t reg );
#endif // !BSP_SPI_H__
