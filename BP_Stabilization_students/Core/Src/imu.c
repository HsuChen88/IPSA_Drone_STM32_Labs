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

void imu_update_attitude(imu_attitude_t* attitude, const imu_scaled_data_t imu_scaled){
	//Compute roll_acc and pitch_acc
	float roll_acc = atan2f(imu_scaled.accel_y, imu_scaled.accel_z) * 180/M_PI;	//from lecture
	float pitch_acc = atan2f(-imu_scaled.accel_x, sqrtf(imu_scaled.accel_y*imu_scaled.accel_y + imu_scaled.accel_z*imu_scaled.accel_z)) * 180/M_PI;

	//Compute roll_gyro and pitch_gyro
	float roll_gyro = attitude->roll + imu_scaled.gyro_x*IMU_SAMPLING_TIME;
	float pitch_gyro = attitude->pitch + imu_scaled.gyro_y*IMU_SAMPLING_TIME;

	//Apply complementary filter
	attitude->roll = IMU_ALPHA*roll_gyro + (1 - IMU_ALPHA)*roll_acc;
	attitude->pitch = IMU_ALPHA*pitch_gyro + (1 - IMU_ALPHA)*pitch_acc;

	//Update rates
	attitude->roll_rate = imu_scaled.gyro_x;
	attitude->pitch_rate = imu_scaled.gyro_y;
	attitude->yaw_rate = imu_scaled.gyro_z;
}
