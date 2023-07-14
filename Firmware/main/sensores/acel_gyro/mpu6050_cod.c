/*
*  Código da TASK de medição do Acelerômetro e Giroscópio com o MPU6050 com os 3 eixos
*  Tópico: MPU6050
*  Project: OBSAT-2023
*  Autor: Kayque Amado
*  Data: 29 de Junho de 2023
*  Creditos: Kolban
*/

#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include "main.h"

#include "sdkconfig.h"

#define PIN_SDA 21
#define PIN_CLK 22
#define I2C_ADDRESS 0x68 // I2C address of MPU6050

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_PWR_MGMT_1   0x6B

static char tag[] = "mpu6050";

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);

void task_mpu6050(void *params) {
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
    vTaskDelay(200/portTICK_PERIOD_MS);

    // Inicialização do MPU6050
    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
    i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1);
    i2c_master_write_byte(cmd, MPU6050_GYRO_XOUT_H, 1);
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
    i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
    i2c_master_write_byte(cmd, 0, 1);
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);

    uint8_t data[14];

    short accel_x;
    short accel_y;
    short accel_z;
    short gyro_x;
    short gyro_y;
    short gyro_z;

    while(1) {
        // Leitura dos valores do acelerômetro
        cmd = i2c_cmd_link_create();
        ESP_ERROR_CHECK(i2c_master_start(cmd));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1));
        ESP_ERROR_CHECK(i2c_master_stop(cmd));
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
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
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
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
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
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
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
        i2c_cmd_link_delete(cmd);

        // Conversão dos valores lidos
        gyro_x = (data[0] << 8) | data[1];
        gyro_y = (data[2] << 8) | data[3];
        gyro_z = (data[4] << 8) | data[5];

        // Impressão dos valores do giroscópio
        printf("gyro_x: %d, gyro_y: %d, gyro_z: %d\n", gyro_x, gyro_y, gyro_z);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void mpu6050_task_start(void)
{
    xTaskCreate(task_mpu6050,"MPU6050", configMINIMAL_STACK_SIZE * 15, NULL, 1, NULL);
}
