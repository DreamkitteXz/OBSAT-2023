idf_component_register(
    SRCS mpu6050_cod.c 
    INCLUDE_DIRS .
    REQUIRES driver/i2c log math freertos 
)
