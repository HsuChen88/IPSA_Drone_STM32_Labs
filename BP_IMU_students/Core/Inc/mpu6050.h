#pragma once

#include "main.h"
#include "imu.h"

#define MPU6050_ADDR 0x68	//7-bit I2C address
#define MPU6050_ADDR_8B (0x68 << 1)	//8-bit I2C address is required when using STM32 HAL Library

#define MPU6050_SMPRT_DIV_REG 0x19
#define MPU6050_CONFIG_REG 0x1A
#define MPU6050_GYRO_CONFIG_REG 0x1B
#define MPU6050_ACCEL_CONFIG_REG 0x1C
#define MPU6050_WHO_AM_I_REG 0x75
#define MPU6050_PWR_MGMT_1_REG 0x6B
#define MPU6050_ACCEL_XOUT_H_REG 0x3B
#define MPU6050_GYRO_XOUT_H_REG 0x43

#define MPU6050_GYRO_LSB 32.8	//32.8 LSB/°/s for FS of +-1000°/s
#define MPU6050_ACCEL_LSB 4096	//4096 LSB/g for FS of +-8g

#define MPU6050_ACCEL_OFFSET_X 0
#define MPU6050_ACCEL_OFFSET_Y 0
#define MPU6050_ACCEL_OFFSET_Z 0
#define MPU6050_GYRO_OFFSET_X 0
#define MPU6050_GYRO_OFFSET_Y 0
#define MPU6050_GYRO_OFFSET_Z 0

uint8_t mpu6050_init();
void mpu6050_read(uint8_t reg, uint8_t len, uint8_t *buf);
void mpu6050_write(uint8_t reg, uint8_t len, uint8_t *buf);
void mpu6050_config();
void mpu6050_gyro_config();
void mpu6050_accel_config();
void mpu6050_pwr_mgmt();
void mpu6050_read_raw_data(imu_raw_data_t* imu_raw);
void mpu6050_read_scaled_data(imu_scaled_data_t* imu_scaled);
