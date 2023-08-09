/*
*  Código da **TASK** da medição de Temperatura com o ds18b20
*  Tópico: DS18B20
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 01 de Julho de 2023
*/

//=======================================================
// --- Bibliiotecas ---

#include "ds18b20.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

//=======================================================
// --- Mapeamento do Hardware ---
#define TEMP_GPIO 15

DeviceAddress tempSensors[2];

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif
float cTemp;

//=======================================================
// --- TASK Principal ---

void ler_temp_ds18b20(void * params) //Fazer uma task
{
   ds18b20_init(TEMP_GPIO);
   ds18b20_setResolution(tempSensors, 2, 10);

   while (1)
   {
      cTemp = ds18b20_get_temp();
      printf("Temperatura: %0.1f C\n", cTemp);
      vTaskDelay(60000 / portTICK_PERIOD_MS);
   }
}

//=======================================================

void ler_temp_ds18b20_start(void)
{
	xTaskCreatePinnedToCore(ler_temp_ds18b20, "ds18b20_temperatura", configMINIMAL_STACK_SIZE * 15, NULL, 5, NULL, APP_CPU_NUM);
}