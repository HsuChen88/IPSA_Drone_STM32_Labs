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
	//TODO: Roll and pitch from scaled accelerometer data in deg/s
	//float roll_acc = ...;
	//float pitch_acc = ...;

	//TODO: Roll and pitch from scaled gyroscope data
	//float roll_gyro = ...;
	//float pitch_gyro = ...;

	//TODO: Apply complementary filter
	//attitude->roll = ...;
	//attitude->pitch = ...;

	//Rates of rotation obtained from the gyroscope
	attitude->roll_rate = imu_scaled.gyro_x;
	attitude->pitch_rate = imu_scaled.gyro_y;
	attitude->yaw_rate = imu_scaled.gyro_z;
}
