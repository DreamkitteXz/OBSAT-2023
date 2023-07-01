/*
*  Código com a Lógica principal do Sistema
*  Tópico: Código Principal
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

//=======================================================
// --- Bibliotecas ---

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h" 
#include "sensores/temperatura_bmp180/bmp180_cod.h" //Lib das tasks do BMP180
#include "sensores/temperatura_ds18b20/ds18b20_cod.h" //lIB DS18B20

//=======================================================
// --- Função Principal ---

void app_main(void)
{
   bmp180_task_start();
   ler_temp_ds18b20_start();
}

//=======================================================
