/*
*  Arquivo com as funções necessárias do MPU6050 para a Lógica principal
*  Tópico: BMP180
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*/

#ifndef MPU6050_TASK_H_ACL
#define MPU6050_TASK_H_ACL

#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>

void task_mpu6050(void *params);
void mpu6050_task_start(void);

#endif // MPU6050_TASK_H