#pragma once

#include "main.h"
#include "imu.h"
#include "motor.h"

#define ARM_LENGTH 0.108	//Distance between each motor and the COG of the bicopter
#define DRONE_MASS 0.210	//Estimated total mass of the bicopter
#define GRAVITY 9.81
#define PULL_MAX 0.66 //Estimated maximum pull delivered by each motor with 3S (0.950kg with 4S)
#define THRUST_MAX PULL_MAX*GRAVITY	//Fmax

#define U1_MAX 7	//Maximum thrust allowed for U1
#define U2_MAX 0.1	//Maximum angular acceleration allowed for U2
#define ALT_I 15	//Initial target altitude after taking off
#define ALT_LAND 7	//Target altitude when landing

typedef struct _control_inputs_t{
	float U1;
	float U2;
}control_inputs_t;

typedef enum _drone_state_e{
	STOPPED,
	FLYING
}drone_state_e;

void control_update(const imu_attitude_t drone_att, const float drone_alt, control_inputs_t* ci);
void control_apply_mixer(const control_inputs_t ci, esc_commands_t* esc_cmd);
