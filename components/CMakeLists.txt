idf_component_register(SRCS "src/user.c"
                            "src/bsp_wifi.c" 
                            "src/bsp_spi.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES nvs_flash
                    )
