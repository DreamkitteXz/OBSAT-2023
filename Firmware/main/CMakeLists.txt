idf_component_register(SRCS "main.c" "sensores/temperatura_ds18b20/ds18b20_cod.c" "sensores/sensores_i2c/i2c.c" "payload/tensao_placa.c" "servidor/wifi.c" "servidor/http_client.c" "payload/contador_geiger.c" "sensores/sensores_i2c/nivel_bateria.c"
                    INCLUDE_DIRS ".")
