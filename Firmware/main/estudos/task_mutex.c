/*
*  Fazendo uns estudos em FreeRTOS na ESP-IDF
*  Tópico: Tasks e Mutexes 
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 20 de Junho de 2023
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

float temperatura;
SemaphoreHandle_t mutexI2C;

// =======================================================
// --- SIMULANDO AS TASKS ---

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

// =======================================================
// --- SIMULANDO O MUTEXI2C ---

float acessa_i2c(comando) // Função de acesso ao I2C
{
   if (comando == 1)
   {
      ESP_LOGI("I2C","Leitura do sensor de Temperatura");
      return 20.0 * ((float) rand() * (float)(RAND_MAX/10));
   }
   else
   {
      ESP_LOGI("I2C", "Escrita no LCD");
      printf("Tela LCD - Temperatura = %f", temperatura);
   }
   return 0;
}

void le_sensor(void * params) //TASK de ler o sensor de temperatura
{
   while (1)
   {
      if(xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)) // Se conseguir "Pegar" o mutex executa a região crítica
      {
         temperatura = acessa_i2c(1);
         ESP_LOGI("Leitura","Temperatura: %f", temperatura);
         xSemaphoreGive(mutexI2C);
      }
      else // Se não mostra uma mensagem de Erro
      {
         ESP_LOGE("Leitura","Não foi possível ler o sensor");
      }
      vTaskDelay(5000 / portTICK_PERIOD_MS); // Aguarda 5 segundos para tentar pegar o Mutex de novo
   }
}

void lcd_display(void * params) //TASK de escrever Temperatura no LCD
{
   while (1)
   {
      if(xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)) // Se conseguir "Pegar" o mutex executa a região crítica
      {
      ESP_LOGI("Display","Escreve no LCD");
      acessa_i2c(0);
      xSemaphoreGive(mutexI2C);
      }
      else // Se não mostra uma mensagem de Erro
      {
         ESP_LOGE("Display","Não foi possível escrever no Display"); 
      }
      vTaskDelay(5000 / portTICK_PERIOD_MS); // Aguarda 5 segundos para tentar pegar o Mutex de novo
   }
}

// =============================================================
// --- Função Principal ---

void app_main(void)
{
   //xTaskCreate(&task1, "leitura", 2048, "Task 1", 1, NULL); 
   //xTaskCreate(&task2, "Display", 2048, "Task 2", 1, NULL);

   mutexI2C = xSemaphoreCreateMutex(); // Criando o MutexI2C
   xTaskCreate(&le_sensor, "Leitura Sensor 1", 2048, NULL, 2, NULL);
   xTaskCreate(&lcd_display, "Atualização do Display", 2048, NULL, 2, NULL);
}
