/*
*  Código da TASK da medição de Temperatura com o bmp180
*  Tópico: BMP180
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

/*
//=======================================================
// --- Bibliotecas ---

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "/components/bmp180/bmp180.h/"

//=======================================================
// --- Task de Leitura da Temperatura ---

void leitura_temperatura(void * params)
{
   while (1)
   {
      
   }
   
}

//=======================================================
// --- Função Principal ---

void app_main(void)
{

}

//=======================================================
*/

#include <inttypes.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <bmp180.h>
#include <string.h>

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

void bmp180_test(void *pvParameters)
{
    bmp180_dev_t dev;
    memset(&dev, 0, sizeof(bmp180_dev_t)); // Zero descriptor

    ESP_ERROR_CHECK(bmp180_init_desc(&dev, 0, 21, 22));
    ESP_ERROR_CHECK(bmp180_init(&dev));

    while (1)
    {
        float temp;
        uint32_t pressure;

        esp_err_t res = bmp180_measure(&dev, &temp, &pressure, BMP180_MODE_STANDARD);
        if (res != ESP_OK)
            printf("Could not measure: %d\n", res);
        else
            /* float is used in printf(). you need non-default configuration in
             * sdkconfig for ESP8266, which is enabled by default for this
             * example. see sdkconfig.defaults.esp8266
             */
            printf("Temperature: %.2f degrees Celsius; Pressure: %" PRIu32 " Pa\n", temp, pressure);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void bmp180_task_start(void)
{
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreatePinnedToCore(bmp180_test, "bmp180_test", configMINIMAL_STACK_SIZE * 15, NULL, 5, NULL, APP_CPU_NUM);
}