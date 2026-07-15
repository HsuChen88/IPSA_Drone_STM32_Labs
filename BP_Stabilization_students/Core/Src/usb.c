#include "usb.h"
#include "motor.h"
#include "control.h"

#include <string.h>
#include <stdlib.h>

extern uint8_t control_state;
usb_packet_t usb_pkt;

/**
 * @brief Process incoming packet received over USB
 * @param usb_packet Struct that stores the received data and the number of bytes
 */
void usb_process_packet(const usb_packet_t usb_packet, pid_t* pid_r, pid_t* pid_z){
	char usb_str[USB_RX_BUFFER_SIZE];
	memcpy(usb_str, usb_packet.data, usb_packet.len);
	usb_str[usb_packet.len] = '\0';

	char* token = strtok(usb_str, ",\r\n");
	if(!token) return;
	char cmd_type = token[0];	//Identify command type

	switch(cmd_type){
	case 'T':	//Take off
		pid_set_target(pid_z, ALT_I);
		control_state = FLYING;
		break;
	case 'L':	//Land
		pid_set_target(pid_z, ALT_LAND);
		break;
	case 'S':	//Stop motors
		esc_commands_t esc_command = {ONESHOT125_MIN, ONESHOT125_MIN};
		motors_set_command(esc_command);
		control_state = STOPPED;
		break;
	case 'Z':	//Target altitude
		token = strtok(NULL, ",\r\n");
		if(!token) return;
		float target_alt = strtof(token, NULL);
		pid_set_target(pid_z, target_alt);
		break;
	case 'G':	//Update PID gains
		uint8_t i = 0;
		float pid_gains[6];
		token = strtok(NULL, ",\r\n");
		while(token != NULL && i < 6){
			pid_gains[i] = strtof(token, NULL);
			i++;
			token = strtok(NULL, ",\r\n");
		}
		if(i == 6){
			usb_update_pid(pid_r, pid_gains[0], pid_gains[1], pid_gains[2]);
			usb_update_pid(pid_z, pid_gains[3], pid_gains[4], pid_gains[5]);
		}
		break;
	case 'P': //Set raw ONESHOT125 pulses
		control_state = STOPPED;
		uint8_t j = 0;
		uint16_t raw_pulses[2];
		token = strtok(NULL, ",\r\n");
		while(token != NULL && j < 2){
			uint32_t val = strtoul(token, NULL, 10);	//Most recent way of getting integers
			if(val > 0xFFFF) return;	//verify the value is within the range of uint16_t
			raw_pulses[j] = (uint16_t)val;
			j++;
			token = strtok(NULL, ",\r\n");
		}
		if(j == 2){
			esc_commands_t esc_command = {.cmd_cw=raw_pulses[0], .cmd_ccw=raw_pulses[1]};
			motors_set_command(esc_command);
		}
		break;
	default:
		break;
	}
}

void usb_update_pid(pid_t *pid, float Kp, float Ki, float Kd){
    pid_t tmp = *pid;
    tmp.Kp = Kp;
    tmp.Ki = Ki;
    tmp.Kd = Kd;

    __disable_irq();
    *pid = tmp;
    __enable_irq();
}

