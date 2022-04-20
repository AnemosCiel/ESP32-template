#ifndef __APP_LED_H__
#define __APP_LED_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include <stdint.h>
#include <stdbool.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
#define APP_LED_HANDLE_MAX 10

typedef enum
{
    APP_LED_SINGLE,
    APP_LED_BLINK,
}APP_LED_MODE;

typedef struct
{
    uint8_t id;
    uint8_t pin;
    uint8_t valid;
    uint8_t level;
    uint8_t mode;
    uint32_t on_time;
    uint32_t off_time;
    uint32_t counter;
}app_led_t;

/*------------------------------Exertn----------------------------------*/
void app_led_core(void);
void app_led_register(uint8_t id, uint8_t pin, uint8_t level, APP_LED_MODE mode);
void app_led_set_time(uint8_t id, uint32_t ontime, uint32_t offtime);
void app_led_delete(uint8_t id);
void app_led_list_id(void);

#endif // !APP_LED_H__
