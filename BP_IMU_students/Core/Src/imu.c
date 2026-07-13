#include "imu.h"
#include "mpu6050.h"

imu_raw_data_t imu_raw;
imu_scaled_data_t imu_scaled;
imu_attitude_t drone_att;

uint8_t imu_init(){
	return mpu6050_init();
}

void imu_read_raw_data(imu_raw_data_t* imu_raw){
	mpu6050_read_raw_data(imu_raw);
}

void imu_read_scaled_data(imu_scaled_data_t* imu_scaled){
	mpu6050_read_scaled_data(imu_scaled);
}

/**
 * @brief Compute the drone's attitude using scaled data from both the accelerometer and the gyroscope
 * @param attitude Struct that will store the updated drone's attitude
 * @param imu_scaled Struct that stores the current IMU scaled data
 */
void imu_update_attitude(imu_attitude_t* attitude, const imu_scaled_data_t imu_scaled){
	static uint8_t gyro_angle_initialized = 0;
	static float gyro_roll = 0.0f;
	static float gyro_pitch = 0.0f;

	float body_accel_x = imu_scaled.accel_x;
	float body_accel_y = imu_scaled.accel_y;
	float body_accel_z = imu_scaled.accel_z;

	//TODO: Roll and pitch from scaled accelerometer data in deg/s
	float roll_acc = atan2f(body_accel_y, body_accel_z) * IMU_RAD_TO_DEG;
	float pitch_acc = -atan2f(body_accel_x, sqrtf((body_accel_y * body_accel_y) + (body_accel_z * body_accel_z))) * IMU_RAD_TO_DEG;

	//TODO: Roll and pitch from scaled gyroscope data
	float roll_rate = imu_scaled.gyro_x;
	float pitch_rate = imu_scaled.gyro_y;

	if(!gyro_angle_initialized){
		gyro_roll = roll_acc;
		gyro_pitch = pitch_acc;
		gyro_angle_initialized = 1;
	}
	else{
		gyro_roll += roll_rate * IMU_SAMPLING_TIME;
		gyro_pitch += pitch_rate * IMU_SAMPLING_TIME;
	}

	attitude->acc_roll = roll_acc;
	attitude->acc_pitch = pitch_acc;
	attitude->gyro_roll = gyro_roll;
	attitude->gyro_pitch = gyro_pitch;

	//TODO: Apply complementary filter
	attitude->roll = gyro_roll;
	attitude->pitch = gyro_pitch;

	//Rates of rotation obtained from the gyroscope
	attitude->roll_rate = roll_rate;
	attitude->pitch_rate = pitch_rate;
	attitude->yaw_rate = imu_scaled.gyro_z;
}
