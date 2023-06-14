/*
Código de leitura da placa solar e dos eventos de radiação
*/

#include <stdio.h>
#include "driver/gpio.h"
#include "esp_sleep.h"

#define PULSE_COUNTER_PIN 12 // Pino GPIO a ser usado pelo contador de pulsos

void app_main()
{
    // Configuração do pino do contador de pulsos
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE; // Conta pulsos na transição de baixo para alto
    io_conf.pin_bit_mask = (1ULL << PULSE_COUNTER_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // Configuração do contador de pulsos
    pcnt_config_t pcnt_config;
    pcnt_config.pulse_gpio_num = PULSE_COUNTER_PIN; // Pino GPIO associado ao contador
    pcnt_config.ctrl_gpio_num = PCNT_PIN_NOT_USED; // Não é necessário um pino de controle adicional
    pcnt_config.unit = PCNT_UNIT_0; // Usar a unidade 0 do contador de pulsos
    pcnt_config.channel = PCNT_CHANNEL_0; // Usar o canal 0 do contador de pulsos
    pcnt_config.pos_mode = PCNT_COUNT_INC; // Contar incrementos no pulso
    pcnt_config.neg_mode = PCNT_COUNT_DIS; // Não contar decrementos
    pcnt_config.lctrl_mode = PCNT_MODE_KEEP; // Manter a contagem no modo de suspensão
    pcnt_config.hctrl_mode = PCNT_MODE_KEEP; // Manter a contagem no modo de suspensão
    pcnt_unit_config(&pcnt_config);

    // Ativar o contador de pulsos
    pcnt_counter_pause(PCNT_UNIT_0, PCNT_CHANNEL_0); // Pausar o contador antes de configurar o valor inicial
    pcnt_counter_clear(PCNT_UNIT_0, PCNT_CHANNEL_0); // Limpar a contagem inicial
    pcnt_counter_resume(PCNT_UNIT_0, PCNT_CHANNEL_0); // Retomar a contagem

    // Colocar o ESP32 em um loop de sono e acordar
    while (1) {
        // Colocar o ESP32 em modo de sono profundo por 4 minutos
        esp_sleep_enable_timer_wakeup(4 * 60 * 1000000); // Acordar após 4 minutos
        esp_deep_sleep_start();
        int16_t data = eventosDeRadiacao();
    }
}

void eventosDeRadiacao(){
    // Depois de acordar do modo de sono
    // Ler o valor do contador de pulsos
    int16_t pulsos;
    pcnt_get_counter_value(PCNT_UNIT_0, &pulsos);

    printf("Número de eventos durante o sono: %d\n", pulsos);

    // Limpar o contador de pulsos
    pcnt_counter_clear(PCNT_UNIT_0, PCNT_CHANNEL_0);

    return pulsos;

}