/*
 * @Description: bsp_pcnt
 * @Author: 
 * @Date: 2022-04-19 14
 * @Path: D:\ESP32\template\components\bsp_pcnt\bsp_pcnt.c
 */
#include <stdio.h>
#include "bsp_pcnt.h"

#include "driver/pcnt.h"

#define BSP_PCNT_COUNT_LIM  30000

typedef struct {
    int64_t count;  // Cumulative count value = overflow times * PCNT_H_LIM_VAL + current PCNT count value
    int64_t overflow;   // Overflow times
    int64_t lastcount;  // last count value
} bsp_pcnt_t;

static bsp_pcnt_t pcnt[PCNT_UNIT_MAX];

 /**
 * @description: pcnt event callback
 * @param:  arg: parameters
 * @return: null
 * @note: 
 */
static void bsp_pcnt_irq_handler(void *arg)
{
    uint8_t unit = (uint8_t)arg;
    uint32_t status;
    
    pcnt_get_event_status(unit, &status);
    if(status & PCNT_EVT_L_LIM)
    {
        pcnt[unit].overflow--;
    }
    if(status & PCNT_EVT_H_LIM)
    {
        pcnt[unit].overflow++;
    }
}

 /**
 * @description: Read pcnt count
 * @param:  unit: pcnt unit
 * @return: count
 * @note: 
 */
int64_t bsp_pcnt_read(uint8_t unit)
{
    int16_t count = 0;
    //获得PCNT计数值
    pcnt_get_counter_value(unit, &count);
    //计算 累计计数值 = 溢出次数*PCNT_H_LIM_VAL + 当前PCNT的计数值
    pcnt[unit].count = pcnt[unit].overflow * BSP_PCNT_COUNT_LIM + count;
    pcnt[unit].lastcount = pcnt[unit].count;
    return pcnt[unit].count;
}

 /**
 * @description: Get pcnt last count
 * @param:  unit: pcnt unit
 * @return: last count
 * @note: 
 */
int64_t bsp_pcnt_get_lastcount(uint8_t unit)
{
    return pcnt[unit].lastcount;
}

 /**
 * @description: Clear pcnt count
 * @param:  unit: pcnt unit
 * @return: null
 * @note: 
 */
void bsp_pcnt_clear(uint8_t unit)
{
    pcnt[unit].count = 0;
    pcnt[unit].overflow = 0;
    pcnt[unit].lastcount = 0;
    pcnt_counter_clear(unit);
}

 /**
 * @description: Start pcnt counting
 * @param:  unit: pcnt unit
 * @return: null
 * @note: 
 */
void bsp_pcnt_start(uint8_t unit)
{
    pcnt_counter_resume(unit);
}

 /**
 * @description: Stop pcnt counting
 * @param:  unit: pcnt unit
 * @return: null
 * @note: 
 */
void bsp_pcnt_stop(uint8_t unit)
{
    pcnt_counter_pause(unit);
}

 /**
 * @description: Initialize pcnt event
 * @param:  unit: pcnt unit
 * @return: null
 * @note: 
 */
static void bsp_pcnt_event_init(uint8_t unit)
{
    /* Set threshold 0 and 1 values and enable events to watch */
#ifdef INTERMEDIATE_THRESHOLD_EN
    pcnt_set_event_value(unit, PCNT_EVT_THRES_1, 5);
    pcnt_event_enable(unit, PCNT_EVT_THRES_1);
    pcnt_set_event_value(unit, PCNT_EVT_THRES_0, -5);
    pcnt_event_enable(unit, PCNT_EVT_THRES_0);
#endif  
#ifdef ZERO_CROSSING_EN
    /* Enable events on zero, maximum and minimum limit values */
    pcnt_event_enable(unit, PCNT_EVT_ZERO);
#endif
    pcnt_event_enable(unit, PCNT_EVT_H_LIM);
    pcnt_event_enable(unit, PCNT_EVT_L_LIM);
}

 /**
 * @description: 1 times count mode
 * @param:  unit: pcnt unit
 *          pin_a: encoder A
 *          pin_b: encoder B
 * @return: null
 * @note: 
 */
void bsp_pcnt_x1_init(uint8_t unit, uint8_t pin_a, uint8_t pin_b)
{
    /* Prepare configuration for the PCNT unit */
    pcnt_config_t pcnt_config = {
        // Set PCNT input signal and control GPIOs
        .pulse_gpio_num = pin_a,
        .ctrl_gpio_num = pin_b,
        .channel = PCNT_CHANNEL_0,
        .unit = unit,
        // What to do on the positive / negative edge of pulse input?
        .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
        .neg_mode = PCNT_COUNT_DIS,   // Keep the counter value on the negative edge
        // What to do when control input is low or high?
        .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
        .hctrl_mode = PCNT_MODE_KEEP,    // Keep the primary counter mode if high
        // Set the maximum and minimum limit values to watch
        .counter_h_lim = BSP_PCNT_COUNT_LIM,
        .counter_l_lim = -BSP_PCNT_COUNT_LIM,
    };
    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_config);

    /* Configure and enable the input filter */
    // 12.5ns*100=1250ns=1.25us，enough to allow 1000000/1.25/2=400KHz signal input
    pcnt_set_filter_value(unit, 100);
    pcnt_filter_enable(unit);

    /* Initialize PCNT's event */
    bsp_pcnt_event_init(unit);

    /* Initialize PCNT's counter */
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);

    /* Install interrupt service and add isr callback handler */
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(unit, bsp_pcnt_irq_handler, (void *)unit);

    /* Everything is set up, now go to counting */
    pcnt_counter_resume(unit);
}

 /**
 * @description: 2 times count mode
 * @param:  unit: pcnt unit
 *          pin_a: encoder A
 *          pin_b: encoder B
 * @return: null
 * @note: 
 */
void bsp_pcnt_x2_init(uint8_t unit, uint8_t pin_a, uint8_t pin_b)
{
    /* Prepare configuration for the PCNT unit */
    pcnt_config_t pcnt_config = {
        // Set PCNT input signal and control GPIOs
        .pulse_gpio_num = pin_a,
        .ctrl_gpio_num = pin_b,
        .channel = PCNT_CHANNEL_0,
        .unit = unit,
        // What to do on the positive / negative edge of pulse input?
        .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
        .neg_mode = PCNT_COUNT_INC,   // Keep the counter value on the negative edge
        // What to do when control input is low or high?
        .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
        .hctrl_mode = PCNT_MODE_KEEP,    // Keep the primary counter mode if high
        // Set the maximum and minimum limit values to watch
        .counter_h_lim = BSP_PCNT_COUNT_LIM,
        .counter_l_lim = -BSP_PCNT_COUNT_LIM,
    };
    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_config);

    /* Configure and enable the input filter */
    // 12.5ns*100=1250ns=1.25us，enough to allow 1000000/1.25/2=400KHz signal input
    pcnt_set_filter_value(unit, 100);
    pcnt_filter_enable(unit);

    /* Initialize PCNT's event */
    bsp_pcnt_event_init(unit);

    /* Initialize PCNT's counter */
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);

    /* Install interrupt service and add isr callback handler */
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(unit, bsp_pcnt_irq_handler, (void *)unit);

    /* Everything is set up, now go to counting */
    pcnt_counter_resume(unit);
}

 /**
 * @description: 4 times count mode
 * @param:  unit: pcnt unit
 *          pin_a: encoder A
 *          pin_b: encoder B
 * @return: null
 * @note: 
 */
void bsp_pcnt_x4_init(uint8_t unit, uint8_t pin_a, uint8_t pin_b)
{
    pcnt_config_t pcnt_config;
    /* Initialize PCNT unit */
    pcnt_config.unit           = unit;
    pcnt_config.lctrl_mode     = PCNT_MODE_REVERSE; // CTRL is low: reverse the counting direction
    pcnt_config.hctrl_mode     = PCNT_MODE_KEEP;    // CTRL is high level: the counting direction does not change
    pcnt_config.counter_h_lim  = BSP_PCNT_COUNT_LIM;    // maximum count
    pcnt_config.counter_l_lim  = -BSP_PCNT_COUNT_LIM;    // Minimum count value

    pcnt_config.pulse_gpio_num = pin_a;
    pcnt_config.ctrl_gpio_num  = pin_b;
    pcnt_config.channel        = PCNT_CHANNEL_0;
    pcnt_config.pos_mode       = PCNT_COUNT_INC;    // rising edge count++
    pcnt_config.neg_mode       = PCNT_COUNT_DEC;    // falling edge count--
    pcnt_unit_config(&pcnt_config);

    pcnt_config.pulse_gpio_num = pin_b;
    pcnt_config.ctrl_gpio_num  = pin_a;
    pcnt_config.channel        = PCNT_CHANNEL_1;
    pcnt_config.pos_mode       = PCNT_COUNT_DEC;    // rising edge count--
    pcnt_config.neg_mode       = PCNT_COUNT_INC;    // falling edge count++
    pcnt_unit_config(&pcnt_config);

    /* Configure and enable the input filter */
    // 12.5ns*100=1250ns=1.25us，enough to allow 1000000/1.25/2=400KHz signal input
    pcnt_set_filter_value(unit, 100);
    pcnt_filter_enable(unit);

    /* Initialize PCNT's event */
    bsp_pcnt_event_init(unit);

    /* Initialize PCNT's counter */
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);

    /* Install interrupt service and add isr callback handler */
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(unit, bsp_pcnt_irq_handler, (void *)unit);

    /* Everything is set up, now go to counting */
    pcnt_counter_resume(unit);
}

 /**
 * @description: pcnt deinit
 * @param:  unit: pcnt unit
 * @return: null
 * @note: 
 */
void bsp_pcnt_deinit(uint8_t unit)
{
    
}


