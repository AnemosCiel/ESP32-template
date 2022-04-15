/*
 * @Description: dev_shtc3
 * @Author: 
 * @Date: 2022-04-15 14
 * @Path: D:\ESP32\template\components\dev_shtc3\dev_shtc3.c
 */
#include <stdio.h>
#include "dev_shtc3.h"
#include "bsp_mi2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef enum{
  READ_ID            = 0xEFC8, // command: read ID register
  SOFT_RESET         = 0x805D, // soft reset
  SLEEP              = 0xB098, // sleep
  WAKEUP             = 0x3517, // wakeup
  MEAS_T_RH_POLLING  = 0x7866, // meas. read T first, clock stretching disabled
  MEAS_T_RH_CLOCKSTR = 0x7CA2, // meas. read T first, clock stretching enabled
  MEAS_RH_T_POLLING  = 0x58E0, // meas. read RH first, clock stretching disabled
  MEAS_RH_T_CLOCKSTR = 0x5C24  // meas. read RH first, clock stretching enabled
}etCommands;

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
static void dev_shtc3_delay_ms(uint32_t ms)
{
    vTaskDelay( pdMS_TO_TICKS( ms ) );
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
bool dev_shtc3_check_crc(uint8_t *data, uint8_t len, uint8_t crc_check)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t cnt;    // byte counter

    // calculates 8-Bit crc_check with given polynomial
    for(cnt = 0; cnt < len; cnt++) 
    {
        crc ^= (data[cnt]);
        for(bit = 8; bit > 0; --bit) 
        {
            if(crc & 0x80) 
            {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } 
            else 
            {
                crc = (crc << 1);
            }
        }
    }

    // verify crc_check
    if(crc != crc_check) 
    {
        return false;
    } 
    else 
    {
        return true;
    }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void dev_shtc3_write_cmd(uint16_t cmd)
{
    uint8_t buf[2] = {0};

    buf[0] = cmd >> 8;
    buf[1] = cmd & 0xFF;

    bsp_mi2c_write(SHTC3_ADDR, buf, sizeof(buf));
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void dev_shtc3_read_data(uint16_t *data)
{
    uint8_t buf[3] = {0};

    bsp_mi2c_read(SHTC3_ADDR, buf, sizeof(buf));

    if(dev_shtc3_check_crc(buf, 2, buf[2]))
    {
        *data = (buf[0] << 8) | buf[1];
    }
    else
    {
        *data = 0;
    }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void dev_shtc3_sleep(void)
{
    dev_shtc3_write_cmd(SLEEP);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void dev_shtc3_wakeup(void)
{
    dev_shtc3_write_cmd(WAKEUP);
}

 /**
 * @description: calculate temperature [°C]
 * @param:  raw value
 * @return: temperature 
 * @note: T = -45 + 175 * rawValue / 2^16   
 */
float  dev_shtc3_calc_temp(uint16_t raw)
{
    return 175 * (float)raw / 65536.0f - 45.0f;
}

 /**
 * @description: calculate relative humidity [%RH]
 * @param:  raw value
 * @return: humidity 
 * @note: RH = rawValue / 2^16 * 100
 */
float  dev_shtc3_calc_humi(uint16_t raw)
{
    return 100 * (float)raw / 65536.0f;
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void dev_shtc3_measurement(float *temp, float *humi)
{
    uint16_t raw_temp = 0, raw_humi = 0;

    dev_shtc3_wakeup();
    dev_shtc3_delay_ms(1);
    dev_shtc3_write_cmd(MEAS_T_RH_POLLING);
    dev_shtc3_delay_ms(20);
    dev_shtc3_read_data(&raw_temp);
    dev_shtc3_read_data(&raw_humi);

    *temp = dev_shtc3_calc_temp(raw_temp);
    *humi = dev_shtc3_calc_humi(raw_humi);
    dev_shtc3_sleep();
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void dev_shtc3_init(void)
{
    
}
