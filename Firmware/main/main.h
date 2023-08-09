/*
*  Código com a Lógica principal do Sistema
*  Tópico: Código Principal
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" 
#include "esp_log.h"

#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"

#include "sensores/temperatura_ds18b20/ds18b20_cod.h"
#include "sensores/sensores_i2c/i2c.h"
#include "payload/tensao_placa.h"
#include "servidor/wifi.h"
#include "servidor/http_client.h"
#include "nvs_flash.h"

void app_main(void);
void save_data_to_eeprom(float data);
float read_data_from_eeprom();



#endif /* MAIN_H */
