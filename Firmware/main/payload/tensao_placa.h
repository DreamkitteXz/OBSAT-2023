/*
*  Arquivo com as funções necessárias de Leitura da tensão da placa (PAYLOAD)
*  Tópico: Leitura Placa
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

#ifndef TENSAO_PLACA_H
#define TENSAO_PLACA_H

#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

void ler_tensao_placa(void*params);
void ler_tensao_placa_start();


#endif //TENSAO_PLACA_H