idf_component_register(
   INCLUDE_DIRS "."
   SRCS tensao_placa.c
   REQUIRES freertos driver esp_adc_cal
)