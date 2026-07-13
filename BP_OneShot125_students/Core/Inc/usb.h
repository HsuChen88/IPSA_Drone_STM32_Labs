#pragma once

#include "main.h"
#include <string.h>
#include <stdio.h>

#define USB_RX_BUFFER_SIZE 64

typedef struct _usb_packet_t{
	uint8_t data[USB_RX_BUFFER_SIZE];
	uint8_t len;
}usb_packet_t;


void usb_process_packet(const usb_packet_t usb_packet);
