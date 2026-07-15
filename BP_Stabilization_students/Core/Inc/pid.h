#pragma once

#include "main.h"

#define I_MAX 1000
#define CONTROL_PERIOD 0.001f	//1ms

typedef struct _pid_t{
	float Kp;
	float Ki;
	float Kd;
	float prev_error;
	float integral;
	float target;
}pid_t;


void pid_init(pid_t* pid);
void pid_set_gains(pid_t* pid, float Kp, float Ki, float Kd);
float pid_update(pid_t* pid, const float state);
void pid_set_target(pid_t* pid, const float target);
