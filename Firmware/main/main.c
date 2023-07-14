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

//Wifi e servidor
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"

//Dados coletados
#include "sensores/temperatura_bmp180/bmp180_cod.h" //Lib das tasks do BMP180
#include "sensores/temperatura_ds18b20/ds18b20_cod.h" //Lib das tasks do DS18B20
#include "sensores/acel_gyro/mpu6050_cod.h" //Lib das tasks do MPU6050 Acl
#include "payload/tensao_placa.h" //Lib da task de ler tensão da placa
#include "servidor/wifi.h" //Lib Wifi 
#include "servidor/http_client.h" // Lib requisição POST

SemaphoreHandle_t mutexI2C;

//=======================================================
// --- Função Principal ---

void app_main(void)
{ 
    //--- inicializar o NVS ---
        esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //Criando o Mutex I2C
    //mutexI2C = xSemaphoreCreateMutex();

    // --- I2C ---
    bmp180_task_start();
    //mpu6050_task_start();

    // --- ONEWIRE ---
    ler_temp_ds18b20_start(); //Inicia a task de ler TEMP DS18B20

    // --- PAYLOAD ---
    ler_tensao_placa_start(); // Inicia a task de ler_tensao_placa

    //--- Wifi ---
    wifi_start();

    //--- HTTP POST ---
    //vTaskDelay(10000 / portTICK_PERIOD_MS); //Aguarda a conexão do wifi
    //http_request();

}
//=======================================================
