/*
 * @Description:
 * @Author:
 * @Date: 2021-12-02 17
 * @Path: D:\ESP32\template\components\src\user.c
 */
#include "user.h"
/***********driver***********/
/* uart */
#include "driver/uart.h"
#include "string.h"
/* gpio */
#include "driver/gpio.h"
/* timer */
#include "esp_system.h"
#include "esp32/rom/ets_sys.h"
#include "esp_timer.h"
/* LowPower */
#include "esp_sleep.h"
#include "soc/rtc.h"
#include "esp_wifi.h"
#include "driver/rtc_io.h"
/* log */
#include "esp_log.h"
/* bsp */
#include "bsp_gpio.h"
#include "bsp_wifi.h"
#include "bsp_spi.h"
#include "bsp_adc.h"
#include "bsp_pwm.h"

QueueHandle_t uartQueue = NULL;
SemaphoreHandle_t uartSemphr = NULL;

static QueueHandle_t edgeQueue = NULL;
QueueHandle_t keyQueue = NULL;

static const char *TAG = "user";

/**
 * @description: led callback function
 * @param:  null
 * @return: null
 * @note:
 */
static void user_led_callback( void *arg )
{
    uint8_t level = 0;
    level = gpio_get_level( hled );
    gpio_set_level( hled, !level );
}

/**
 * @description: init user led
 * @param:  null
 * @return: null
 * @note:
 */
static void user_led_init( void )
{
    /* simple way
    gpio_pad_select_gpio(hled);
    gpio_reset_pin(hled);
    gpio_set_direction(hled, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(hled, USER_LED_ON);
    */
    gpio_config_t config =
    {
        .intr_type = GPIO_PIN_INTR_DISABLE, /* Interrupt disabled */
        .mode = GPIO_MODE_INPUT_OUTPUT,     /* Input an output mode */
        .pin_bit_mask = ( 1ULL << hled ),   /* GPIO pin number */
        .pull_down_en = 0,                  /* Pull down disable */
        .pull_up_en = 0,                    /* pull up disable */
    };
    gpio_config( &config );
    const esp_timer_create_args_t led_timer_args =
    {
        .callback = &user_led_callback,
        /* name is optional, but may help identify the timer when debugging */
        .name = "led"
    };
    esp_timer_handle_t led_timer;
    ESP_ERROR_CHECK( esp_timer_create( &led_timer_args, &led_timer ) );
    ESP_ERROR_CHECK( esp_timer_start_periodic( led_timer, USER_LED_SPEED * 1000 ) );
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void user_key_isrcallback( void *arg )
{
    /* Read pin level */
    uint8_t edge = gpio_get_level( hkey );
    xQueueSendFromISR( edgeQueue, &edge, NULL );
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void user_key_scan( void *arg )
{
    key_event_t key_event = {0};
    uint8_t edge = 0;
    while( 1 )
    {
        if( xQueueReceive( edgeQueue, &edge, portMAX_DELAY ) )
        {
            key_event.edge = edge;
            key_event.current_time = user_get_systemtick();
            if( key_event.edge )
            {
                key_event.rising_time = key_event.current_time;
            }
            else
            {
                key_event.falling_time = key_event.current_time;
            }
            if( key_event.edge )
            {
                key_event.duration_time = key_event.rising_time - key_event.falling_time;
                if( key_event.duration_time > USER_KEY_JITTER * 1000 )
                {
                    if( key_event.duration_time > USER_KEY_LONGPRESS * 1000 * 1000 )
                    {
                        /* Key long press */
                        key_event.press_type = 2;
                    }
                    else
                    {
                        /* Key short press */
                        key_event.press_type = 1;
                    }
                    xQueueSend( keyQueue, &key_event.press_type, 10 );
#if KEY_INFO
                    ESP_LOGI( TAG, "KeyPress, Duration: %lld ms, Type: %d\n", key_event.duration_time / 1000, key_event.press_type );
#endif
                }
                else
                {
                    /* No key press */
                    key_event.press_type = 0;
                }
            }
        }
    }
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void user_key_init( void )
{
    gpio_config_t config =
    {
        .intr_type = GPIO_INTR_ANYEDGE, /* Both rising and falling edge */
        .mode = GPIO_MODE_INPUT,        /* Input mode */
        .pin_bit_mask = ( 1ULL << hkey ), /* GPIO pin number */
        .pull_down_en = 0,              /* Pull down disable */
        .pull_up_en = 1,                /* pull up enable */
    };
    gpio_config( &config );
    /* There is jitter, internal use of secondary processing */
    edgeQueue = xQueueCreate( 10, sizeof( uint32_t ) );
    /* Provided for external use */
    keyQueue = xQueueCreate( 10, sizeof( uint32_t ) );
    gpio_install_isr_service( 0 );
    gpio_isr_handler_add( hkey, user_key_isrcallback, ( void * )hkey );
    xTaskCreate( user_key_scan, "key_task", 4096, NULL, 10, NULL );
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void user_uart_sendstr( char *data )
{
    uint32_t txbytes = uart_write_bytes( huart, data, strlen( data ) );
#if UART_INFO
    ESP_LOGI( TAG, "Wrote %d bytes", txbytes );
#endif
    uart_write_bytes( huart, "\r\n", 2 );
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void user_uart_task( void *arg )
{
    uart_event_t event;
    while( 1 )
    {
        /* Waiting for UART event. */
        if( xQueueReceive( uartQueue, ( void * )&event, ( portTickType )portMAX_DELAY ) )
        {
            uint8_t dtmp[1024] = {0};
#if UART_INFO
            ESP_LOGI( TAG, "uart[%d] event:", huart );
#endif
            switch( event.type )
            {
                case UART_DATA:
                    /* Event of UART receving data */
                    uart_read_bytes( huart, dtmp, event.size, portMAX_DELAY );
#if UART_INFO
                    ESP_LOGI( TAG, "[UART DATA]: %d", event.size );
                    ESP_LOGI( TAG, "[DATA EVT]:%s", dtmp );
#endif
                    if( strstr( ( const char * )dtmp, "OK" ) != NULL )
                    {
                        xSemaphoreGive( uartSemphr );
                    }
                    break;
                case UART_FIFO_OVF:
                    /* Event of HW FIFO overflow detected */
#if UART_INFO
                    ESP_LOGI( TAG, "hw fifo overflow" );
#endif
                    uart_flush_input( huart );
                    xQueueReset( uartQueue );
                    break;
                case UART_BUFFER_FULL:
                    /* Event of UART ring buffer full */
#if UART_INFO
                    ESP_LOGI( TAG, "ring buffer full" );
#endif
                    uart_flush_input( huart );
                    xQueueReset( uartQueue );
                    break;
                case UART_BREAK:
                    /* Event of UART RX break detected */
#if UART_INFO
                    ESP_LOGI( TAG, "uart rx break" );
#endif
                    break;
                case UART_PARITY_ERR:
                    /* Event of UART parity check error */
#if UART_INFO
                    ESP_LOGI( TAG, "uart parity error" );
#endif
                    break;
                case UART_FRAME_ERR:
                    /* Event of UART frame error */
#if UART_INFO
                    ESP_LOGI( TAG, "uart frame error" );
#endif
                    break;
                case UART_PATTERN_DET:
                    /* UART_PATTERN_DET */
#if UART_INFO
                    ESP_LOGI( TAG, "uart pattern det" );
#endif
                    break;
                default:
#if UART_INFO
                    ESP_LOGI( TAG, "uart event type: %d", event.type );
#endif
                    break;
            }
        }
    }
}

/**
 * @description: init user uart
 * @param:  null
 * @return: null
 * @note:
 */
static void user_uart_init( void )
{
    uart_config_t uart_config =
    {
        .baud_rate = 115200,                   // baud rate
        .data_bits = UART_DATA_8_BITS,         // data bits
        .parity = UART_PARITY_DISABLE,         // parity check bits
        .stop_bits = UART_STOP_BITS_1,         // stop bits
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // hard ware control
        .source_clk = UART_SCLK_APB,           // source clock from APB
    };
    uart_param_config( huart, &uart_config );
    uart_set_pin( huart,
                  USER_UART_TX,
                  USER_UART_RX,
                  USER_UART_RTS,
                  USER_UART_CTS );
    uart_driver_install(
        huart,                // uart number
        USER_UART_TXBUF_SIZE, // tx buf size
        USER_UART_RXBUF_SIZE, // rx buf size
        USER_UART_QUEUE_SIZE, // queue size
        &uartQueue,           // queue handle
        0                     // interrupt flag
    );
    uartSemphr = xSemaphoreCreateBinary();
    xTaskCreate( user_uart_task, "uart_task", 1024 * 4, NULL, 12, NULL );
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void user_lowpwoer( void )
{
    esp_wifi_stop();
    /* System will restart when awake from sleep mode */
    esp_deep_sleep( DEEPSLEEP_TIME * 1000000 );
}

/**
 * @description: user task
 * @param:  null
 * @return: null
 * @note:
 */
void user_task( void *arg )
{
    char InfoBuffer[512] = {0};
    while( 1 )
    {
        printf( "heap size:%d\n", esp_get_free_heap_size() );
        vTaskList( ( char * )&InfoBuffer );
        printf( "任务名      任务状态  优先级   剩余栈 任务序号(R:Ready   B:Block    S:Suspend)\r\n" );
        printf( "%s\r\n", InfoBuffer );
        vTaskGetRunTimeStats( ( char * )&InfoBuffer );
        printf( "\r\n任务名          运行计数         使用率\r\n" );
        printf( "%s\r\n", InfoBuffer );
        vTaskDelay( pdMS_TO_TICKS( 2000 ) );
    }
}

/**
 * @description: user init task
 * @param:  null
 * @return: null
 * @note:
 */
void user_init( void )
{
    user_led_init();
    user_key_init();
    // bsp_pwm_init();
    // bsp_adc_init();
    // user_uart_init();
    // bsp_wifi_init();
    // user_lowpwoer();
#if TASK_INFO
    /* Show task info */
    // xTaskCreate(user_task, "user_task", 4096, NULL, 10, NULL);
#endif
}