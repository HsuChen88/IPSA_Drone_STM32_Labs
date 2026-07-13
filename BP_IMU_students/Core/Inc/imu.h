#pragma once

#include "main.h"
#include <math.h>

#define IMU_SAMPLING_TIME 0.001	//1kHz
#define IMU_ALPHA 0.9

#define IMU_FLAG 0x01

typedef struct _imu_raw_data_t{
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
}imu_raw_data_t;

typedef struct _imu_scaled_data_t{
	float accel_x;
	float accel_y;
	float accel_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
}imu_scaled_data_t;

typedef struct _imu_attitude_t{
	float roll;
	float pitch;
	float yaw;
	float roll_rate;
	float pitch_rate;
	float yaw_rate;
}imu_attitude_t;


uint8_t imu_init();
void imu_read_raw_data(imu_raw_data_t* imu_raw);
void imu_read_scaled_data(imu_scaled_data_t* imu_scaled);
void imu_update_attitude(imu_attitude_t* attitude, const imu_scaled_data_t);

