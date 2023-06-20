/*
*  Fazendo uns estudos em FreeRTOS na ESP-IDF
*  Tópico: Semáforo Binário
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

SemaphoreHandle_t semaforoBinario;

//=======================================================
// --- SIMULANDO CONEXÃO COM O SERVIDOR ---

void conexao_servidor(void * params) // Task de conexão com Servidor
{
   while (1)
   {
      ESP_LOGI("Servidor", "Conectar ao servidor");
      xSemaphoreGive(semaforoBinario);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
   }
}

//=======================================================
// --- SÓ VAI SER EXECUTADA SE O SEMÁFORO LIBERAR ---

void processa_dados(void * params) // Task de processamento de Dados
{
    while (1)
   {
      xSemaphoreTake(semaforoBinario, portMAX_DELAY);
      printf("Página Carregada!\n");
   }
}

// =============================================================
// --- Função Principal ---

void app_main(void)
{  
   semaforoBinario = xSemaphoreCreateBinary(); // Criando o Semáforo Binário
   xTaskCreate(&conexao_servidor, "Conexao com o servidor", 2048, NULL, 1, NULL);
   xTaskCreate(&processa_dados, "Processa do dados", 2048, NULL, 1, NULL);
}
