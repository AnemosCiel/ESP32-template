/*
 * @Description: 
 * @Author: YZ
 * @Date: 2022-02-25 16
 * @Path: D:\ESP32\template\components\bsp_pwm\bsp_pcnt.c
 */
#include "bsp_pcnt.h"
#include "driver/pcnt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

/* A sample structure to pass events from the PCNT
 * interrupt handler to the main program.
 */
typedef struct {
    int unit;  // the PCNT unit that originated an interrupt
    uint32_t status; // information on the event type that caused the interrupt
} pcnt_evt_t;

static const char *TAG = "pcnt";
xQueueHandle pcnt_evt_queue;   // A queue to handle pulse counter events

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
static void bsp_pcnt_irq_handler(void *arg)
{
    int pcnt_unit = (int)arg;
    pcnt_evt_t evt;
    evt.unit = pcnt_unit;
    /* Save the PCNT event type that caused an interrupt
       to pass it to the main program */
    pcnt_get_event_status(pcnt_unit, &evt.status);
    xQueueSendFromISR(pcnt_evt_queue, &evt, NULL);
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
static void bsp_pcnt_task(void *arg)
{
    int16_t count = 0;
    pcnt_evt_t evt;
    portBASE_TYPE res;
    pcnt_evt_queue = xQueueCreate(10, sizeof(pcnt_evt_t));

    while (1) 
    {
        /* Wait for the event information passed from PCNT's interrupt handler.
         * Once received, decode the event type and print it on the serial monitor.
         */
        res = xQueueReceive(pcnt_evt_queue, &evt, 1000 / portTICK_PERIOD_MS);
        if (res == pdTRUE) {
            pcnt_get_counter_value(evt.unit, &count);
            ESP_LOGI(TAG, "Event PCNT unit[%d]; cnt: %d", evt.unit, count);
            if (evt.status & PCNT_EVT_THRES_1) {
                ESP_LOGI(TAG, "THRES1 EVT");
            }
            if (evt.status & PCNT_EVT_THRES_0) {
                ESP_LOGI(TAG, "THRES0 EVT");
            }
            if (evt.status & PCNT_EVT_L_LIM) {
                ESP_LOGI(TAG, "L_LIM EVT");
            }
            if (evt.status & PCNT_EVT_H_LIM) {
                ESP_LOGI(TAG, "H_LIM EVT");
            }
            if (evt.status & PCNT_EVT_ZERO) {
                ESP_LOGI(TAG, "ZERO EVT");
            }
        } else {
            pcnt_get_counter_value(evt.unit, &count);
            ESP_LOGI(TAG, "Current counter value :%d", count);
        }
    }
}


 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_pwm_pcnt_init(uint8_t unit, uint8_t pin, uint8_t ctl_pin)
{
    /* Prepare configuration for the PCNT unit */
    pcnt_config_t pcnt_config = {
        // Set PCNT input signal and control GPIOs
        .pulse_gpio_num = pin,
        .ctrl_gpio_num = ctl_pin,
        .channel = PCNT_CHANNEL_0,
        .unit = unit,
        // What to do on the positive / negative edge of pulse input?
        .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
        .neg_mode = PCNT_COUNT_DIS,   // Keep the counter value on the negative edge
        // What to do when control input is low or high?
        .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
        .hctrl_mode = PCNT_MODE_KEEP,    // Keep the primary counter mode if high
        // Set the maximum and minimum limit values to watch
        .counter_h_lim = BSP_PCNT_COUNT_LIMIT,
        .counter_l_lim = -BSP_PCNT_COUNT_LIMIT,
    };
    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_config);

    /* Configure and enable the input filter */
    pcnt_set_filter_value(unit, BSP_PCNT_FILTER_VALUE);
    pcnt_filter_enable(unit);

    /* Set threshold 0 and 1 values and enable events to watch */
    pcnt_set_event_value(unit, PCNT_EVT_THRES_1, BSP_PCNT_EVT_THRES_1);
    pcnt_event_enable(unit, PCNT_EVT_THRES_1);
    pcnt_set_event_value(unit, PCNT_EVT_THRES_0, BSP_PCNT_EVT_THRES_0);
    pcnt_event_enable(unit, PCNT_EVT_THRES_0);
    /* Enable events on zero, maximum and minimum limit values */
    pcnt_event_enable(unit, PCNT_EVT_ZERO);
    pcnt_event_enable(unit, PCNT_EVT_H_LIM);
    pcnt_event_enable(unit, PCNT_EVT_L_LIM);

    /* Initialize PCNT's counter */
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);

    /* Install interrupt service and add isr callback handler */
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(unit, bsp_pcnt_irq_handler, (void *)unit);

    /* Everything is set up, now go to counting */
    pcnt_counter_resume(unit);

    xTaskCreate(bsp_pcnt_task, "bsp_pcnt_task", 1024, NULL, 3, NULL);
}
