idf_component_register(
    SRCS ds18b20_cod.c
    INCLUDE_DIRS .
    REQUIRES ds18b20 driver/gpio freertos
)
