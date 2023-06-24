/*
*  Fazendo uns estudos em FreeRTOS na ESP-IDF
*  Tópico: Queues (Filas)
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 20 de Junho de 2023
*/

//Não esta Funcionando :(
   
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
QueueHandle_t fila_de_temperaturas;

//=======================================================
// --- QUEUES ---

void leitura_de_sensores(void * params) //TASK de leitura do sensor de temperatura
{
   float temperatura;
   while (true)
   {
      //Leitura do sensor de temperatura
      temperatura = 20.0 * ((float) rand() / (float)(RAND_MAX / 10));
      long resposta = xQueueSend(fila_de_temperaturas, &temperatura, 1000 / portTICK_PERIOD_MS);
      if (resposta)
      {
         ESP_LOGI("Leitura","Temperatura adcionada à fila");
      }
      else
      {
         ESP_LOGE("Leitura","Falha no envio da temperatura");
      }
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   }
}

void calcula_media_movel(void * params) //TASK de calculo da média da temperatura 
{
   float temperatura = 0.0;
   float temperatura_anterior = 0.0;
   while (1)
   {
      if (xQueueReceive(fila_de_temperaturas, &temperatura, 5000 / portTICK_PERIOD_MS))
      {
      temperatura_media = (temperatura_anterior * temperatura) / 2;
      temperatura_anterior = temperatura;
      printf("Temperatura média %f\n", temperatura_media);
      }
      else
      {
         ESP_LOGE("Calculo","Fila não disponível");
      }
      
   }
}

// =============================================================
// --- Função Principal ---

void app_main(void)
{
   fila_de_temperaturas = xQueueCreate(5, sizeof(float));
   xTaskCreate(&leitura_de_sensores, "Le Temperatura", 2048, NULL, 1, NULL);
   xTaskCreate(&calcula_media_movel, "Calcula temperatura média movel", 2048, NULL, 2, NULL);
}