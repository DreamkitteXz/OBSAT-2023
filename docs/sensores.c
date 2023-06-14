/*
Demostração de funcionamento das funções que acho necessárias na nossa missão 
Author: Otávio
Data: 13/06/2023
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "mpu6050.h"
#include "bmp180.h"
#include "hmc5883l.h"

#define I2C_MASTER_SCL_IO           19         // Pino GPIO para o clock (SCL) do I2C
#define I2C_MASTER_SDA_IO           18         // Pino GPIO para o dado (SDA) do I2C
#define I2C_MASTER_NUM              I2C_NUM_0  // Número do barramento I2C
#define I2C_MASTER_FREQ_HZ          100000     // Frequência de comunicação I2C (100kHz)

mpu6050_t mpu;  // Estrutura para armazenar dados do MPU6050
bmp180_t bmp;  // Estrutura para armazenar dados do BMP180
hmc5883l_t hmc;  // Estrutura para armazenar dados do HMC5883L

void initialize_sensors() {
    i2c_port_t i2c_master_port = I2C_MASTER_NUM;  // Define o número do barramento I2C a ser usado
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,  // Configura o modo do barramento I2C como mestre
        .sda_io_num = I2C_MASTER_SDA_IO,  // Define o pino GPIO para o dado (SDA) do I2C
        .sda_pullup_en = GPIO_PULLUP_ENABLE,  // Habilita o resistor de pull-up para o pino SDA
        .scl_io_num = I2C_MASTER_SCL_IO,  // Define o pino GPIO para o clock (SCL) do I2C
        .scl_pullup_en = GPIO_PULLUP_ENABLE,  // Habilita o resistor de pull-up para o pino SCL
        .master.clk_speed = I2C_MASTER_FREQ_HZ  // Define a frequência de comunicação do I2C
    };
    i2c_param_config(i2c_master_port, &i2c_config);  // Configura o barramento I2C com as opções definidas
    i2c_driver_install(i2c_master_port, i2c_config.mode, 0, 0, 0);  // Instala o driver I2C com as configurações especificadas

    mpu6050_init(&mpu, i2c_master_port);  // Inicializa o sensor MPU6050 com o barramento I2C especificado
    bmp180_init(&bmp, i2c_master_port);  // Inicializa o sensor BMP180 com o barramento I2C especificado
    hmc5883l_init(&hmc, i2c_master_port);  // Inicializa o sensor HMC5883L com o barramento I2C especificado
}

void read_sensors_task(void *pvParameters) {
    while (1) {
        mpu6050_data_t accel_data, gyro_data;
        bmp180_data_t temp_pressure_data;
        hmc5883l_data_t mag_data;

        mpu6050_read_accel(&mpu, &accel_data);  // Leitura dos dados do acelerômetro do MPU6050
        mpu6050_read_gyro(&mpu, &gyro_data);  // Leitura dos dados do giroscópio do MPU6050
        bmp180_read_temp_pressure(&bmp, &temp_pressure_data);  // Leitura dos dados de temperatura e pressão do BMP180
        hmc5883l_read_mag(&hmc, &mag_data);  // Leitura dos dados do magnetômetro do HMC5883L

        printf("Acelerômetro (m/s^2): %.2f, %.2f, %.2f\n", accel_data.x, accel_data.y, accel_data.z);  // Impressão dos dados do acelerômetro
        printf("Giroscópio (rad/s): %.2f, %.2f, %.2f\n", gyro_data.x, gyro_data.y, gyro_data.z);  // Impressão dos dados do giroscópio
        printf("Temperatura (°C): %.2f\n", temp_pressure_data.temperature);  // Impressão dos dados de temperatura
        printf("Pressão (hPa): %.2f\n", temp_pressure_data.pressure);  // Impressão dos dados de pressão
        printf("Campo Magnético (uT): %.2f, %.2f, %.2f\n", mag_data.x, mag_data.y, mag_data.z);  // Impressão dos dados do magnetômetro
        printf("-------------------------\n");

        vTaskDelay(pdMS_TO_TICKS(1000));  // Aguarda 1 segundo antes de fazer a próxima leitura
    }
}

void app_main() {
    initialize_sensors();  // Inicialização dos sensores

    xTaskCreate(read_sensors_task, "read_sensors_task", 2048, NULL, 10, NULL);  // Criação da tarefa para leitura dos sensores
}