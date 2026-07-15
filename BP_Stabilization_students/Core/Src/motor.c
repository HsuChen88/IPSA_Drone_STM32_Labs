#include "motor.h"

extern TIM_HandleTypeDef htim1;

/**
 * @brief Start PWM with 125us pulses
 */
void motors_init(){
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ONESHOT125_MIN);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ONESHOT125_MIN);
}

/**
 * @brief Set CCR values within the admissible range
 * @param esc_cmd Custom struct that stores the commands to send to each ESC
 */
void motors_set_command(const esc_commands_t esc_cmd){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, clamp(esc_cmd.cmd_cw, ONESHOT125_MIN, ONESHOT125_MAX_SAFE));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, clamp(esc_cmd.cmd_ccw, ONESHOT125_MIN, ONESHOT125_MAX_SAFE));
}
