/*
*  Fazendo uns estudos em FreeRTOS na ESP-IDF
*  Tópico: Queues (Filas)
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 20 de Junho de 2023
*/

//=======================================================
// --- Bibliotecas ---

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

float temperatura_media;

//=======================================================

void leitura_de_sensores()
{
   float temperatura;
   while (true)
   {
      //Leitura do sensor de temperatura
   }
   
}

// =============================================================
// --- Função Principal ---

void app_main(void)
{

}