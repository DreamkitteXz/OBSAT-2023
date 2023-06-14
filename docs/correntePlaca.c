#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/adc.h>
#include <esp_sleep.h>

#define V_REF 3300  // Tensão de referência em milivolts (3.3V)
#define ADC_BITS 12 // Bits de resolução do conversor ADC (12 bits)
#define ADC_MAX_VALUE ((1 << ADC_BITS) - 1) // Valor máximo do conversor ADC

#define ACS758_SENSITIVITY 0.185 // Sensibilidade do ACS758 em mV/A

void adc_task(void *pvParameters) {
        int adc_value = adc1_get_raw(ADC1_CHANNEL_6); // Lê o valor do ADC

        //adc_power_off(); // Desativa o ADC

        // Converte o valor ADC para tensão
        float voltage = (adc_value * V_REF) / ADC_MAX_VALUE;

        // Converte a tensão para corrente
        float current = voltage / ACS758_SENSITIVITY;

        printf("Corrente: %.2f A\n", current);

        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF); // Desativa o domínio RTC_PERIPH para economizar energia
    }
}

void setOnACD(){
    adc1_config_width(ADC_WIDTH_BIT_12); // Configura a resolução do ADC para 12 bits
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // Configura a entrada analógica do ADC (GPIO 34)

    adc_power_on(); // Ativa o ADC
}

void app_main() {
    esp_sleep_enable_timer_wakeup(pdMS_TO_TICKS(1000)); // Configura o tempo de espera para 1 segundo

    xTaskCreate(adc_task, "adc_task", 2048, NULL, 5, NULL);

    esp_deep_sleep_start(); // Entra em modo de baixo consumo de energia
}
