idf_component_register(
    SRCS ds18b20.c
    REQUIRES driver freertos esp_timer
    INCLUDE_DIRS "."
    )