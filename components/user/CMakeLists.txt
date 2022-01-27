idf_component_register(SRCS "usystem.c" "user.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES bsp_gpio bsp_wifi bsp_spi bsp_adc bsp_pwm
                    )
