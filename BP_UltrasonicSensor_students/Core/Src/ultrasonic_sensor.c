#include "ultrasonic_sensor.h"

uint32_t us_dma_buffer[2];	//[0]: counter value at rising_edge, [1]: counter value at falling edge
float us_dist = 0;	//raw ultrasonic sensor distance (in cm)
uint8_t us_data_ready = 0;	//New measurement from input capture
uint32_t us_pulse_width = 0;	//ultrasonic sensor propagation time (in microseconds)
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;


/**
 * @brief This function computes the distance (in cm) between the sensor and the detected obstacle
 */
void us_compute_distance(){
	uint32_t rising = us_dma_buffer[0];
	uint32_t falling = us_dma_buffer[1];

	//Propagation time computation
	if(falling >= rising)
	  us_pulse_width = falling - rising;
	else
	  us_pulse_width = (__HAL_TIM_GET_AUTORELOAD(&htim5) - rising + 1) + falling;	//Considers counter overflow


	printf("us_pulse_width: %.1f \n", us_pulse_width);
	//TODO: distance computation
	us_dist = us_pulse_width * 0.0343f / 2.0f;	//Distance in cm
}


/**
 * @brief This function sets the trigger pin of the US to HIGH level to start the emission of ultrasonic waves
 */
void us_enable_trigger(){
	//Enable trigger pin for 10us
	HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_RESET);
}
