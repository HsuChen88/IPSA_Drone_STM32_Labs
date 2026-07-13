#include "mpu6050.h"

extern I2C_HandleTypeDef hi2c2;

/**
 * @brief Initialize the device
 */
uint8_t mpu6050_init(){
	uint8_t dev_addr;
	mpu6050_read(MPU6050_WHO_AM_I_REG, 1, &dev_addr);
	if(dev_addr != MPU6050_ADDR)
		return 0;
	mpu6050_pwr_mgmt();
	mpu6050_config();
	mpu6050_gyro_config();
	mpu6050_accel_config();
	return 1;
}

/**
 * @brief Configure the PWR_MGMT_1 Register
 * @note First reset the device then disable the temperature sensor and select the clock source.
 */
void mpu6050_pwr_mgmt(){
	//Reset the device first
	uint8_t data = 0x80;
	mpu6050_write(MPU6050_PWR_MGMT_1_REG, 1, &data);
	HAL_Delay(100);	//wait at least 100ms to let the device perform the reset

	//Then disable temperature sensor and select the clock source
	data = 0x0A;
	mpu6050_write(MPU6050_PWR_MGMT_1_REG, 1, &data);
	HAL_Delay(10);
}

/**
 * @brief Configure the CONFIG register
 */
void mpu6050_config(){
	uint8_t data = 0x03;	//Bandwidth: 44Hz for accel, 42Hz for gyro
	mpu6050_write(MPU6050_CONFIG_REG, 1, &data);
}

/**
 * @brief Configure the GYRO_CONFIG register
 */
void mpu6050_gyro_config(){
	uint8_t data = 0x10;	//FS: +-1000°/s
	mpu6050_write(MPU6050_GYRO_CONFIG_REG, 1, &data);
}

/**
 * @brief Configure the ACCEL_CONFIG register
 */
void mpu6050_accel_config(){
	uint8_t data = 0x10;	//FS: +-8g
	mpu6050_write(MPU6050_ACCEL_CONFIG_REG, 1, &data);
}

/**
 * @brief Read the most recent raw data provided by the accelerometer and the gyroscope
 * @param imu_raw Struct that will store these data
 */
void mpu6050_read_raw_data(imu_raw_data_t* imu_raw){
	uint8_t data[6];
	mpu6050_read(MPU6050_ACCEL_XOUT_H_REG, 6, data);
	imu_raw->accel_x = ((data[0]<<8) | data[1]) - MPU6050_ACCEL_OFFSET_X;
	imu_raw->accel_y = ((data[2]<<8) | data[3]) - MPU6050_ACCEL_OFFSET_Y;
	imu_raw->accel_z = ((data[4]<<8) | data[5]) - (MPU6050_ACCEL_OFFSET_Z - MPU6050_ACCEL_LSB); //Z should be 1g when the sensor is in rest

	mpu6050_read(MPU6050_GYRO_XOUT_H_REG, 6, data);
	imu_raw->gyro_x = ((data[0]<<8) | data[1]) - MPU6050_GYRO_OFFSET_X;
	imu_raw->gyro_y = ((data[2]<<8) | data[3]) - MPU6050_GYRO_OFFSET_Y;
	imu_raw->gyro_z = ((data[4]<<8) | data[5]) - MPU6050_GYRO_OFFSET_Z;
}

/**
 * @brief Read the most recent scaled data provided by the accelerometer and the gyroscope
 * @param imu_scaled Struct that will store these data
 */
void mpu6050_read_scaled_data(imu_scaled_data_t* imu_scaled){
	imu_raw_data_t imu_raw;
	mpu6050_read_raw_data(&imu_raw);

	//Accelero
	imu_scaled->accel_x = (float)imu_raw.accel_x / MPU6050_ACCEL_LSB;
	imu_scaled->accel_y = (float)imu_raw.accel_y / MPU6050_ACCEL_LSB;
	imu_scaled->accel_z = (float)imu_raw.accel_z / MPU6050_ACCEL_LSB;

	//Gyro
	imu_scaled->gyro_x = (float)imu_raw.gyro_x / MPU6050_GYRO_LSB;
	imu_scaled->gyro_y = (float)imu_raw.gyro_y / MPU6050_GYRO_LSB;
	imu_scaled->gyro_z = (float)imu_raw.gyro_z / MPU6050_GYRO_LSB;
}

/**
 * @brief Read data from 1 or several registers of the device
 * @param reg First register address you want to read data from
 * @param len Number of consecutive bytes you want to read
 * @param buff Pointer that will store the data transmitted by the device
 */
void mpu6050_read(uint8_t reg, uint8_t len, uint8_t *buf){
	HAL_I2C_Mem_Read(&hi2c2, MPU6050_ADDR_8B, reg, 1, buf, len, 100);	//1 is the number of bytes per register (for this device, we only have 8 bits)
}

/**
 * @brief Write data to one or several registers of the device
 * @param reg First register address you want to write data to
 * @param len Number of consecutive bytes to write
 * @param buf Buffer that stores the bytes to write to the register(s)
 */
void mpu6050_write(uint8_t reg, uint8_t len, uint8_t *buf){
	HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDR_8B, reg, 1, buf, len, 100);	//1 is the number of bytes per register (for this device, we only have 8 bits)
}
