idf_component_register(
    SRCS i2c.c nivel_bateria.c 
        INCLUDE_DIRS .
    REQUIRES driver/i2c log math freertos 
)
