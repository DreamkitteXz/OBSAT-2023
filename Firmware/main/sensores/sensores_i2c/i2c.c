#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"
#include <math.h>
#include "main.h"
#include <inttypes.h>
#include <esp_system.h>
#include <bmp180.h>
#include <string.h>
#include "driver/i2c.h"

#include "sdkconfig.h"

#define PIN_SDA 21
#define PIN_CLK 22
#define I2C_ADDRESS 0x68 // I2C address of MPU6050

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_PWR_MGMT_1   0x6B

static char tag[] = "mpu6050";

SemaphoreHandle_t i2cMutex;

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x) do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);

float temperatura;

void acessa_bmp180() {
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

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void acessa_mpu6050() {
    ESP_LOGD(tag, ">> mpu6050");

    // Configuração do barramento I2C
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = PIN_SDA;
    conf.scl_io_num = PIN_CLK;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

    i2c_cmd_handle_t cmd;
    vTaskDelay(200 / portTICK_PERIOD_MS);

    // Inicialização do MPU6050
    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
    i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1);
    i2c_master_write_byte(cmd, MPU6050_GYRO_XOUT_H, 1);
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
    i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
    i2c_master_write_byte(cmd, 0, 1);
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);

    uint8_t data[14];

    short accel_x;
    short accel_y;
    short accel_z;
    short gyro_x;
    short gyro_y;
    short gyro_z;

    while (1) {
        // Leitura dos valores do acelerômetro
        cmd = i2c_cmd_link_create();
        ESP_ERROR_CHECK(i2c_master_start(cmd));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1));
        ESP_ERROR_CHECK(i2c_master_stop(cmd));
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS));
        i2c_cmd_link_delete(cmd);

        cmd = i2c_cmd_link_create();
        ESP_ERROR_CHECK(i2c_master_start(cmd));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_READ, 1));

        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data,   0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+1, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+2, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+3, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+4, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+5, 1));

        ESP_ERROR_CHECK(i2c_master_stop(cmd));
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS));
        i2c_cmd_link_delete(cmd);

        // Conversão dos valores lidos
        accel_x = (data[0] << 8) | data[1];
        accel_y = (data[2] << 8) | data[3];
        accel_z = (data[4] << 8) | data[5];

        // Impressão dos valores do acelerômetro
        printf("accel_x: %d, accel_y: %d, accel_z: %d\n", accel_x, accel_y, accel_z);

        // Leitura dos valores do giroscópio
        cmd = i2c_cmd_link_create();
        ESP_ERROR_CHECK(i2c_master_start(cmd));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, MPU6050_GYRO_XOUT_H, 1));
        ESP_ERROR_CHECK(i2c_master_stop(cmd));
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS));
        i2c_cmd_link_delete(cmd);

        cmd = i2c_cmd_link_create();
        ESP_ERROR_CHECK(i2c_master_start(cmd));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_READ, 1));

        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data,   0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+1, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+2, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+3, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+4, 0));
        ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+5, 1));

        ESP_ERROR_CHECK(i2c_master_stop(cmd));
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS));
        i2c_cmd_link_delete(cmd);

        // Conversão dos valores lidos
        gyro_x = (data[0] << 8) | data[1];
        gyro_y = (data[2] << 8) | data[3];
        gyro_z = (data[4] << 8) | data[5];

        // Impressão dos valores do giroscópio
        printf("gyro_x: %d, gyro_y: %d, gyro_z: %d\n", gyro_x, gyro_y, gyro_z);

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void sensor_bmp180(void *params) {
    while (1) {
        if (i2cMutex != NULL) {
            if (xSemaphoreTake(i2cMutex, 1000 / portTICK_PERIOD_MS)) {
                acessa_bmp180();
                xSemaphoreGive(i2cMutex);
            } else {
                ESP_LOGE("BMP180", "Não foi possível ler o sensor");
            }
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void sensor_mpu6050(void *params) {
    while (1) {
        if (i2cMutex != NULL) {
            if (xSemaphoreTake(i2cMutex, 1000 / portTICK_PERIOD_MS)) {
                acessa_mpu6050();
                xSemaphoreGive(i2cMutex);
            } else {
                ESP_LOGE("MPU6050", "Não foi possível ler o sensor");
            }
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void i2c_start() {
    i2cMutex = xSemaphoreCreateMutex(); // Criando o Mutex para o sensor BMP180

    //xTaskCreate(&sensor_bmp180, "Leitura Sensor BMP180", 2048, NULL, 2, NULL);
    xTaskCreate(&sensor_mpu6050, "Leitura Sensor MPU6050", 2048, NULL, 2, NULL);
}
