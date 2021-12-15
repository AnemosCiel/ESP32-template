/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
/**************Include******************/
/* Library */
#include "esp_system.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <time.h>
/* FreeRTOS */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* log */
#include "esp_log.h"
#include "esp_event.h"

/* LowPower */
#include "esp_sleep.h"
#include "soc/rtc.h"
/* User */
#include "user.h"

/**************Define******************/


void app_main(void)
{
   user_init();
   //vTaskDelay(100);
}
