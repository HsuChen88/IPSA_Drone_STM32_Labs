#pragma once

#include "main.h"
#include "pid.h"
#include <string.h>
#include <stdio.h>

#define USB_RX_BUFFER_SIZE 64

typedef struct _usb_packet_t{
	uint8_t data[USB_RX_BUFFER_SIZE];
	uint8_t len;
}usb_packet_t;


void usb_process_packet(const usb_packet_t usb_packet, pid_t* pid_r, pid_t* pid_z);
void usb_update_pid(pid_t *pid, float Kp, float Ki, float Kd);
