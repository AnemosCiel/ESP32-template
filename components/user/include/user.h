#ifndef __USER_H__
#define __USER_H__

/*  NOTICE
    1.This is some pin only can be used as input
        pin num:34,35,36,39
    2.This is some pin can be used as SPI,so can`t be used as other
        pin num:6,7,8,9,10,11(CLK,SD0,SD1,SD2,SD3,CMD)
    3.This is some pin used as touch pad
        pin num:4,0,2,15,13,12,14,27,33,32(T0~T9)
    4.This is some pin can be used as ADC
        pin num:36,37,38,39,32,33,34,35,4,0,2,15,13,12,14,27,25,26(ADC1:CH0~CH7,ADC2:CH0~CH9)
    5.This is some pin can be used as DAC
        pin num:25,26(DAC1,DAC2)
    6. This is some pin can`t be used as PWM
        pin num:GPIO 34~39
    7.This is some pin can be used as SPI
        VSPI(SPI2):MOSI(23), MISO(19), SCLK(18), CS(5)
        HSPI(SPI3):MOSI(13), MISO(12), SCLK(14), CS(15)

*/
/*-----------------------------Include-----------------------------------*/
/***********lib***********/
#include "sdkconfig.h"
#include <stdint.h>
#include <stdio.h>
/* rtos */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/*------------------------------Define----------------------------------*/
/***********debug config***********/
#define KEY_INFO    1
#define UART_INFO   0
#define TASK_INFO   1
#define TICK_INFO   1
/* Deep sleep */
#define DEEPSLEEP_TIME 10 // unit: second
/***********led***********/
#define hled            25 /* LED pin number */

#define USER_LED_ON     1
#define USER_LED_OFF    0

#define USER_LED_SPEED  1000 /* units: ms */
/***********key***********/
#define hkey                0   /* Key pin number */
#define USER_KEY_JITTER     8   /* Jitter time (unit:ms) */
#define USER_KEY_LONGPRESS  1   /* Key long press time (unit:s) */

typedef struct
{
    uint8_t edge;                     /* The falling edge is 0 and the rising edge is 1*/
    int64_t current_time;              /* Record the current trigger system time */
    int64_t rising_time, falling_time; /* Record the system time when rising edge and falling edge trigger */
    int64_t duration_time;             /* The duration of a button being pressed */
    uint8_t press_type;                /* Key event type */
} key_event_t;

/***********uart***********/
#define huart UART_NUM_1

#define USER_UART_TX 4 /* Any port except can only be used as input */
#define USER_UART_RX 0
#define USER_UART_RTS UART_PIN_NO_CHANGE
#define USER_UART_CTS UART_PIN_NO_CHANGE

#define USER_UART_TXBUF_SIZE 1024
#define USER_UART_RXBUF_SIZE 1024
#define USER_UART_QUEUE_SIZE 10

/*------------------------------Exertn----------------------------------*/
extern SemaphoreHandle_t uartSemphr;

void user_init( void );
void user_uart_sendstr( char *data );
int64_t user_get_systemtick( void );
int32_t user_get_runtime( int64_t star );
#endif // !__USER_H/