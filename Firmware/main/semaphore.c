#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

SemaphoreHandle_t semaforoBinario;

void conexao_servidor(void * params) // Task de Escrever no Display
{
   while (1)
   {
      ESP_LOGI("Servidor", "Conectar ao servidor");
      xSemaphoreGive(semaforoBinario);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
   }
}
void processa_dados(void * params)
{
    while (1)
   {
      xSemaphoreTake(semaforoBinario, portMAX_DELAY);
      printf("Página Carregada!\n");
   }
}

void app_main(void)
{  
   semaforoBinario = xSemaphoreCreateBinary();
   xTaskCreate(&conexao_servidor, "Conexao com o servidor", 2048, NULL, 1, NULL);
   xTaskCreate(&processa_dados, "Processa do dados", 2048, NULL, 1, NULL);
}
