/*
*  Arquivo com as funções necessárias do DS18B20 para a Lógica principal (PAYLOAD)
*  Tópico: DS18B20 (Temperatura da Placa Solar)
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

#ifndef DS18B20_TASK_H
#define DS18B20_TASK_H

#include "ds18b20.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

void ler_temp_ds18b20(void * params);
void ler_temp_ds18b20_start(void);

#endif // DS18B20_TASK_H