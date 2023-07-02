/*
*  Código da TASK de Medição de eventos do Contador Geiger (PAYLOAD)
*  Tópico: Contador Geiger
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" 
#include "esp_log.h"
#include "driver/pulse_cnt.h"

#define EXAMPLE_PCNT_HIGH_LIMIT 100
#define EXAMPLE_PCNT_LOW_LIMIT  -100
#define EXAMPLE_CHAN_GPIO_A 25

void ler_contador(void *params)
{

   pcnt_unit_config_t unit_config = {
    .high_limit = EXAMPLE_PCNT_HIGH_LIMIT,
    .low_limit = EXAMPLE_PCNT_LOW_LIMIT,
   };
   pcnt_unit_handle_t pcnt_unit = NULL;
   ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

   pcnt_chan_config_t chan_config = {
    .edge_gpio_num = EXAMPLE_CHAN_GPIO_A,
   };
   pcnt_channel_handle_t pcnt_chan = NULL;
   ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config, &pcnt_chan));

   // decrease the counter on rising edge, increase the counter on falling edge
   ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
   // keep the counting mode when the control signal is high level, and reverse the counting mode when the control signal is low level
   ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
   
   int pulse_count = 0;
   ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));
   printf("Valor do contador: %d", pulse_count);
   
}