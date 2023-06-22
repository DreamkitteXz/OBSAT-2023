
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
#include "esp_log.h"
#include "driver/gpio.h"

//=======================================================
// --- ---

#define BOTAO_PCNT 34
#define LED_1 27
#define PIN_3V3 32

//=======================================================
// --- Função Principal ---

void app_main(void)
{
   //Configuração dos pinos
   esp_rom_gpio_pad_select_gpio(BOTAO_PCNT);
   esp_rom_gpio_pad_select_gpio(PIN_3V3);
   esp_rom_gpio_pad_select_gpio(LED_1);

   // Configura Led como Output
   gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
   gpio_set_direction(PIN_3V3, GPIO_MODE_OUTPUT);
   //Configura o Botão como Input
   gpio_set_direction(BOTAO_PCNT, GPIO_MODE_INPUT);
   gpio_pulldown_en(BOTAO_PCNT);
   gpio_pullup_dis(BOTAO_PCNT); 
   
   //Teste LED
   gpio_set_level(LED_1, 1);
   gpio_set_level(PIN_3V3, 1);
   
   while (1)
   {
      int estado_botao = gpio_get_level(BOTAO_PCNT);
      gpio_set_level(LED_1, estado_botao);
      vTaskDelay(100 / portTICK_PERIOD_MS);
   }
   
}