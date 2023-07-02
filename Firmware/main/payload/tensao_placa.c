/*
*  Código da medição da Corrente gerada pela placa (PAYLOAD)
*  Tópico: Sensor de Corrente
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

static esp_adc_cal_characteristics_t adc1_chars;

void ler_tensao_placa(void*params)
{

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);

    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);

    while (1) 
    {
        int adc_value = adc1_get_raw(ADC1_CHANNEL_4);
        float adc_value_voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_4), &adc1_chars);
        float corrente = adc_value_voltage / 193;
        printf("Valor ADC: %d", adc_value); 
        printf("\n");
        printf("Valor ADC em volts: %f V\n ", adc_value_voltage); 
        printf("Valor da corrente da placa: %0.2f mA\n", corrente);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
    }
}

void ler_tensao_placa_start()
{
    xTaskCreate(&ler_tensao_placa,"Leitura de tensao da placa", 2048, NULL, 1, NULL);
}