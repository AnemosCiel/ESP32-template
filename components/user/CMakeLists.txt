idf_component_register(SRCS "user.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES bsp_wifi bsp_spi bsp_adc
                    )
