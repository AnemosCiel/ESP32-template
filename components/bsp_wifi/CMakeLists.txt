idf_component_register(SRCS "bsp_server.c" "bsp_sta.c" "bsp_ap.c" "bsp_scan.c"
                    INCLUDE_DIRS "include"
                    REQUIRES user nvs_flash esp_http_server json
                    EMBED_FILES "index.html")
