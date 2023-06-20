#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

void task1(void * params) // Task de Leitura dos sensores
{
   while (1) 
   {
      ESP_LOGI("TASK1", "Leitura de Sensores\n");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   }
}
void task2(void * params) // Task de Escrever no Display
{
   while (1)
   {
      ESP_LOGI("TASK2", "Escrever no display\n");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
   }
}

void app_main(void)
{
   xTaskCreate(&task1, "leitura", 2048, "Task 1", 1, NULL);
   xTaskCreate(&task2, "Display", 2048, "Task 2", 1, NULL);
}
