#pragma once

#include "main.h"

#define ONESHOT125_MIN 125
#define ONESHOT125_MAX 250
#define ONESHOT125_MIN_SENT 132	//Prevent motors from being stopped during the stabilization (5% of their max velocity)
#define ONESHOT125_MAX_SAFE 190	//Prevent the motors from spinning too fast (36% of their max velocity)

typedef struct _esc_commands_t{
	uint16_t cmd_ccw;
	uint16_t cmd_cw;
}esc_commands_t;

void motors_init();
void motors_set_command(const esc_commands_t esc_cmd);
