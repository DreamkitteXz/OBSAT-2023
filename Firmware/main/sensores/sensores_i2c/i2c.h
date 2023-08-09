/*
*  Header file da TASK de medição do Acelerômetro e Giroscópio com o MPU6050 com os 3 eixos
*  Tópico: MPU6050
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*  Creditos: Kolban
*/

#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"
#include <math.h>
#include "main.h"
#include <inttypes.h>
#include <stdio.h>
#include <esp_system.h>
#include <bmp180.h>
#include <string.h>

#define PIN_SDA 21
#define PIN_CLK 22
#define I2C_ADDRESS 0x68 // I2C address of MPU6050

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_PWR_MGMT_1   0x6B

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);

void acessa_i2c(int comando);

void sensor_mpu6050(void *params);

void i2c_start();

#endif  // SENSOR_TASK_H
