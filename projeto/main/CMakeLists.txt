set(requires esp-tls nvs_flash esp_event esp_netif esp_http_client esp_driver_gpio)

idf_build_get_property(target IDF_TARGET)

if(${target} STREQUAL "linux")
    list(APPEND requires esp_stubs protocol_examples_common)
endif()
idf_component_register(SRCS "program.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES ${requires}
                    EMBED_TXTFILES cert.pem)
