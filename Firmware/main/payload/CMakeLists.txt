idf_component_register(
   INCLUDE_DIRS "."
   SRCS tensao_placa.c
   SRCS contador_geiger.c
   SRCS nivel_bateria.c 
   REQUIRES freertos driver esp_adc_cal
)