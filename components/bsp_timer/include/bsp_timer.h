#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

/*-----------------------------Include-----------------------------------*/
/*----------lib---------*/
#include  <stdint.h>
#include  <stdbool.h>
/*----------driver---------*/
/*------------------------------Define----------------------------------*/
/**
 * @brief Timer user data, will be pass to timer alarm callback
 */
typedef struct {
    int timer_group;
    int timer_idx;
    int alarm_value;
    bool auto_reload;
} timer_user_data_t;

#define BSP_TIMER_FREQUENCY 8000 //HZ
/*------------------------------Exertn----------------------------------*/

#endif // !BSP_TIMER_H__
