/*
*  Código com a Lógica principal do Sistema
*  Tópico: Código Principal
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/
//=======================================================
// --- Bibliotecas ---

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" 
#include "esp_log.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"

//
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

//Wifi e servidor
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"

//Dados coletados
#include "sensores/temperatura_ds18b20/ds18b20_cod.h" //Lib das tasks do DS18B20
#include "sensores/sensores_i2c/i2c.h" //Lib das tasks do MPU6050 e do BMP180
#include "payload/tensao_placa.h" //Lib da task de ler tensão da placa
#include "payload/contador_geiger.h"
#include "servidor/wifi.h" //Lib Wifi 
#include "servidor/http_client.h" // Lib requisição POST
#include <bmp180.h>

float temperatura;


//=======================================================
// --- Função Principal ---
static esp_adc_cal_characteristics_t adc1_chars;


void ler_nivel_bateria(void*params)
{

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);

    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);

    while (1) 
    {
        int adc_value = adc1_get_raw(ADC1_CHANNEL_7);
        float adc_value_voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_7), &adc1_chars);
        //float corrente = adc_value_voltage / 193;
        printf("Valor ADC bateria: %d", adc_value); 
        printf("\n");
        printf("Valor ADC em volts bateria: %f V\n ", adc_value_voltage); 
        //printf("Valor da corrente da placa: %0.2f mA\n", corrente);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
    }
}


void app_main(void)
{ 
    //--- inicializar o NVS ---
        esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    
    /*

    ESP_ERROR_CHECK(ret);
     bmp180_dev_t dev;
    memset(&dev, 0, sizeof(bmp180_dev_t)); // Zero descriptor

    ESP_ERROR_CHECK(bmp180_init_desc(&dev, 0, PIN_SDA, PIN_CLK));
    ESP_ERROR_CHECK(bmp180_init(&dev));

    while (1) {
        float temp;
        uint32_t pressure;

        esp_err_t res = bmp180_measure(&dev, &temp, &pressure, BMP180_MODE_STANDARD);
        if (res != ESP_OK)
            printf("Não foi possível medir: %d\n", res);
        else
            printf("Temperatura: %.2f Celsius; Pressão: %.2f atm\n", temp, pressure * 0.00000986923);

        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
    */
    //ler_temp_ds18b20_start();

    //Criando o Mutex I2C
    //mutexI2C = xSemaphoreCreateMutex();

    // xTaskCreate(&ler_nivel_bateria,"Leitura de tensao da placa", 2048, NULL, 1, NULL);

    // --- I2C ---
    //i2c_start();

    // --- ONEWIRE ---
    //ler_temp_ds18b20_start();
    // --- PAYLOAD ---
    //ler_tensao_placa_start(); // Inicia a task de ler_tensao_placa

    //acessa_bmp180();

   


    //pulse_counter_start();

    //--- Wifi ---
    //wifi_start();

    //--- HTTP POST ---
    //vTaskDelay(10000 / portTICK_PERIOD_MS); //Aguarda a conexão do wifi
    //http_request();

    //=============================================
    /*
    while (1)
    {
    
    
    nvs_handle_t my_handle;
    nvs_open("Storage", NVS_READWRITE, &my_handle);
    int32_t read_data;
    nvs_get_i32(my_handle, "data", &read_data);
    printf("Read Data: %ld\n", read_data);
    //----------------------------------------------
    //write
    printf("INT DS18B20 %d", (int)ds18b20_get_temp());
    int32_t write_data = (int)ds18b20_get_temp();
    nvs_set_i32(my_handle, "data", write_data);
    printf("Write data:%ld\n", write_data);
    nvs_commit(my_handle);
    nvs_close(my_handle);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    */
}
//=======================================================
