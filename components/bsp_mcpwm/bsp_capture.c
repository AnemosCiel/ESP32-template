/*
 * @Description: 
 * @Author: YZ
 * @Date: 2022-02-25 15
 * @Path: D:\ESP32\template\components\bsp_mcpwm\bsp_capture.c
 */

#include "bsp_capture.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define CAP0_INT_EN BIT(27)  //Capture 0 interrupt bit
#define CAP1_INT_EN BIT(28)  //Capture 1 interrupt bit
#define CAP2_INT_EN BIT(29)  //Capture 2 interrupt bit

typedef struct {
    uint8_t unit;
    uint32_t capture_signal;
    mcpwm_capture_signal_t sel_cap_signal;
} capture;

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
static void bsp_capture_irq_handle(void *arg)
{
    // uint32_t mcpwm_intr_status;
    
    // if(MCPWM0.int_st.val != 0)
    // {
    //     evt.unit = 0;
    //     mcpwm_intr_status = MCPWM0.int_st.val; //Read interrupt status
    // }
    // else if(MCPWM1.int_st.val != 0)
    // {
    //     evt.unit = 1;
    //     mcpwm_intr_status = MCPWM1.int_st.val; //Read interrupt status
    // }

    // if (mcpwm_intr_status & CAP0_INT_EN) { //Check for interrupt on rising edge on CAP0 signal
    //     evt.capture_signal = mcpwm_capture_signal_get_value(MCPWM_UNIT_0, MCPWM_SELECT_CAP0); //get capture signal counter value
    //     evt.sel_cap_signal = MCPWM_SELECT_CAP0;
    // }
    // if (mcpwm_intr_status & CAP1_INT_EN) { //Check for interrupt on rising edge on CAP1 signal
    //     evt.capture_signal = mcpwm_capture_signal_get_value(MCPWM_UNIT_0, MCPWM_SELECT_CAP1); //get capture signal counter value
    //     evt.sel_cap_signal = MCPWM_SELECT_CAP1;
    // }
    // if (mcpwm_intr_status & CAP2_INT_EN) { //Check for interrupt on rising edge on CAP2 signal
    //     evt.capture_signal = mcpwm_capture_signal_get_value(MCPWM_UNIT_0, MCPWM_SELECT_CAP2); //get capture signal counter value
    //     evt.sel_cap_signal = MCPWM_SELECT_CAP2;
    // }
}

 /**
 * @description: 
 * @param:  null
 * @return: null
 * @note: 
 */
void bsp_capture_register(uint8_t unit, uint8_t channel, uint8_t edge, uint8_t pin)
{
    mcpwm_gpio_init(unit, (MCPWM_CAP_0 + channel), pin);
    gpio_pulldown_en(pin);    //Enable pull down on CAP signal

    mcpwm_isr_register(unit, bsp_capture_irq_handle, NULL, ESP_INTR_FLAG_IRAM, NULL);  //Set ISR Handler
    //pulse num = 0 i.e. 800,000,000 counts is equal to one second
    if(edge)
    {
        mcpwm_capture_enable(unit, (MCPWM_SELECT_CAP0 + channel), MCPWM_POS_EDGE, 0); //capture signal on rising edge
    }
    else
    {
        mcpwm_capture_enable(unit, (MCPWM_SELECT_CAP0 + channel), MCPWM_NEG_EDGE, 0); //capture signal on rising edge
    }
}
