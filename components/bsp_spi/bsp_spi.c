/*
 * @Description:
 * @Author:
 * @Date: 2021-12-07 13
 * @Path: D:\ESP32\template\components\src\bsp_spi.c
 */
#include "bsp_spi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "sdkconfig.h"
#include "esp_log.h"

static const char *TAG = "bsp_spi";
spi_device_handle_t hspi;

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
esp_err_t bsp_spi_write(uint8_t *data, uint8_t len)
{
    esp_err_t ret;
    spi_transaction_t transfer;

    if (len == 0)
        return ESP_ERR_INVALID_SIZE;        // no need to send anything
    memset(&transfer, 0, sizeof(transfer)); // Zero out the transaction

    transfer.flags = SPI_TRANS_USE_TXDATA;
    transfer.length = len * 8;                  // Len is in bytes, transaction length is in bits.
    transfer.tx_buffer = data;                  // Data
    transfer.user = (void *)1;                  // D/C needs to be set to 1
    ret = spi_device_transmit(hspi, &transfer); // Transmit!
    assert(ret == ESP_OK);                      // Should have had no issues.
#if SPI_INFO
    ESP_LOGI(TAG, "send result: %d", ret);
#endif
    return ret;
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
esp_err_t bsp_spi_read(uint8_t *data, uint8_t len)
{
    esp_err_t ret;
    spi_transaction_t transfer;

    memset(&transfer, 0, sizeof(transfer));

    transfer.length = len * 8;
    transfer.flags = SPI_TRANS_USE_RXDATA;
    transfer.rx_buffer = data;
    transfer.user = (void *)1; // D/C needs to be set to 1
    ret = spi_device_transmit(hspi, &transfer);
    assert(ret == ESP_OK);
#if SPI_INFO
    for (uint8_t i = 0; i < len; i++)
    {
        ESP_LOGI(TAG, "send: %d", data[i]);
    }
#endif
    return ret;
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_spi_transfer(uint8_t cmd, uint8_t addr, uint8_t *data, uint8_t *rxbuf, uint8_t len)
{
    spi_transaction_t transfer;

    memset(&transfer, 0, sizeof(transfer));

    transfer.cmd = cmd;
    transfer.addr = addr;
    transfer.tx_buffer = data;
    transfer.rx_buffer = rxbuf;
    transfer.length = len;

    esp_err_t ret = spi_device_polling_transmit(hspi, &transfer);
    assert(ret == ESP_OK);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_spi_init(void)
{
    esp_err_t err;

    spi_bus_config_t buscfg = {
        .miso_io_num = USER_SPI_MISO,
        .mosi_io_num = USER_SPI_MOSI,
        .sclk_io_num = USER_SPI_SCLK,
        .quadwp_io_num = USER_SPI_QUADWP,
        .quadhd_io_num = USER_SPI_QUADHD,
        .max_transfer_sz = USER_SPI_MAX_TRANSFER_SIZE,
    };

    spi_device_interface_config_t devcfg = {
        //.command_bits = 8,
        //.address_bits = 24,
        .clock_speed_hz = USER_SPI_FREQUENCY,
        .mode = USER_SPI_MODE,
        .spics_io_num = USER_SPI_CS,
        .queue_size = USER_SPI_QUEUE_SEZE,
    };

    err = spi_bus_initialize(USER_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(err);
    err = spi_bus_add_device(USER_SPI_HOST, &devcfg, &hspi);
    ESP_ERROR_CHECK(err);
}
