idf_component_register(SRCS "usystem.c" "user.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES bsp_gpio bsp_wifi bsp_spi bsp_mi2c bsp_adc bsp_ledc bsp_mcpwm bsp_now 
                    dev_shtc3
                    app_led
                    )
