#include "pid.h"

void pid_init(pid_t* pid){
	pid->Kp = 0.0;
	pid->Ki = 0.0;
	pid->Kd = 0.0;
	pid->integral = 0.0;
	pid->prev_error = 0.0;
	pid->target = 0.0;
}

/**
 * @brief Set the gains of the PID controller.
 * @param pid Custom struct that stores the parameters used by a PID controller.
 * @param Kp Proportional gain.
 * @param Ki Integral gain.
 * @param Kd Derivative gain.
 * @retval None
 */
void pid_set_gains(pid_t* pid, float Kp, float Ki, float Kd){
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
}


/**
 * @brief Execute the PID according to the current state.
 * @param pid Custom struct that stores the parameters used by a PID controller.
 * @param state Current state.
 * @retval PID output.
 */
float pid_update(pid_t* pid, const float state){
	float error = pid->target - state;
	float derivative = (error - pid->prev_error) / CONTROL_PERIOD;
	pid->prev_error = error;

	pid->integral += error * CONTROL_PERIOD;
	if(pid->integral > I_MAX)
		pid->integral = I_MAX;
	else if (pid->integral < -I_MAX)
		pid->integral = -I_MAX;

	return pid->Kp*error + pid->Kd*derivative + pid->Ki*pid->integral;
}


/**
 * @brief Set a target for the corresponding PID controller.
 * @param pid Custom struct that stores the parameters used by a PID controller.
 * @param target Target state.
 * @retval None
 */
void pid_set_target(pid_t* pid, const float target){
	pid->target = target;
}
