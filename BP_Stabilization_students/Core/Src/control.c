#include "control.h"
#include "pid.h"


pid_t pid_roll;
pid_t pid_alt;
control_inputs_t control_inputs;
uint8_t control_state = STOPPED;

void control_update(const imu_attitude_t drone_att, const float drone_alt, control_inputs_t* ci){
	//Compute U1 using the altitude PID controller: U1 = m*(Kp*e + Kd*de/dt + Ki*int(e)) + m*g
	if(drone_alt > 0.0f)
		//TODO: Compute U1 using altitude PID controller
		ci->U1 = DRONE_MASS * pid_update(&pid_alt, drone_alt) + DRONE_MASS * GRAVITY;

	//Constrain U1 (DO NOT TOUCH)
	ci->U1 = clampf(ci->U1, 0, U1_MAX);

	//TODO: Compute U2 using roll PID controller. The inertia matrix is not considered (it is unknown)
	//Compute U2 using roll PID controller. The inertia matrix is not considered (it is unknown), so Ixx is absorbed into the PID gains
	//drone_att.roll is in degrees; convert to radians so the PID operates in rad
	ci->U2 = pid_update(&pid_roll, drone_att.roll*M_PI/180);

	//Constrain U2 (DO NOT TOUCH)
	ci->U2 = clampf(ci->U2, -U2_MAX, U2_MAX);
}

/**
 * @brief Apply the mixer to determine the :commands to send to each ESC
 * @param ci Custom struct that stores the control inputs U1 and U2
 */
void control_apply_mixer(const control_inputs_t ci, esc_commands_t* esc_cmd){
	//Compute F1 and F2 using U1, U2 and L
	float F_ccw = (ci.U1 / 2.0f) - (ci.U2 / (2.0f * ARM_LENGTH));
	float F_cw  = (ci.U1 / 2.0f) + (ci.U2 / (2.0f * ARM_LENGTH));

	//Ensure F1 and F2 are in the range [0, Fmax] (DO NOT TOUCH)
	F_ccw = clampf(F_ccw, 0, THRUST_MAX);
	F_cw = clampf(F_cw, 0, THRUST_MAX);

	//Compute the adequate pulse widths (DO NOT TOUCH)
	uint16_t pulse_ccw = map_uint16(F_ccw*100, 0, THRUST_MAX*100, ONESHOT125_MIN, ONESHOT125_MAX);
	uint16_t pulse_cw = map_uint16(F_cw*100, 0, THRUST_MAX*100, ONESHOT125_MIN, ONESHOT125_MAX);

	//Constrain the pulse width to make sure the motors do not drain too much current (DO NOT TOUCH)
	pulse_ccw = clamp(pulse_ccw, ONESHOT125_MIN_SENT, ONESHOT125_MAX_SAFE);
	pulse_cw = clamp(pulse_cw, ONESHOT125_MIN_SENT, ONESHOT125_MAX_SAFE);

	//Set the target OneShot125 pulses (DO NOT TOUCH)
	esc_cmd->cmd_ccw = pulse_ccw;
	esc_cmd->cmd_cw = pulse_cw;
}
