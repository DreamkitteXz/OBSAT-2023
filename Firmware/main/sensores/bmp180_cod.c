/*
*  Código da TASK da medição de Temperatura com o bmp180
*  Tópico: BMP180
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
#include "/components/bmp180/bmp180.h/"

//=======================================================
// --- Task de Leitura da Temperatura ---

void leitura_temperatura(void * params)
{
   while (1)
   {
      
   }
   
}

//=======================================================
// --- Função Principal ---

void app_main(void)
{

}

//=======================================================
