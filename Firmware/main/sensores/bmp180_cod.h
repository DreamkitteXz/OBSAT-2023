/*
*  Arquivo com as funções necessárias do bmp180 para a Lógica principal
*  Tópico: BMP180
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

/*
*  Código da TASK da medição de Temperatura com o BMP180
*  Tópico: BMP180
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

#ifndef BMP180_TASK_H_
#define BMP180_TASK_H_

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

void bmp180_test(void *pvParameters);
void bmp180_task_start(void);

#endif /* BMP180_TASK_H_ */
