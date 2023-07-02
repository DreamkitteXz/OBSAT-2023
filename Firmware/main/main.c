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
#include "sensores/temperatura_bmp180/bmp180_cod.h" //Lib das tasks do BMP180
#include "sensores/temperatura_ds18b20/ds18b20_cod.h" //Lib das tasks do DS18B20
#include "sensores/acel_gyro/mpu6050_cod.h"//Lib das tasks do MPU6050
#include "payload/tensao_placa.h"

//=======================================================
// --- Função Principal ---

void app_main(void)
{
   bmp180_task_start();
   ler_temp_ds18b20_start();
   //PARA UTILIZAR O BMP180 E MPU6050 TEMOS QUE CRIAR UM MUTEX
   ler_tensao_placa_start();
}

//=======================================================
