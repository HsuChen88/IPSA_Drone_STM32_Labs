#include "usb.h"
#include "motor.h"

#include <string.h>
#include <stdlib.h>

extern uint8_t control_state;
usb_packet_t usb_pkt;

/**
 * @brief Process incoming packet received over USB
 * @param usb_packet Struct that stores the received data and the number of bytes
 */
void usb_process_packet(const usb_packet_t usb_packet){
	char usb_str[USB_RX_BUFFER_SIZE];
	memcpy(usb_str, usb_packet.data, usb_packet.len);
	usb_str[usb_packet.len] = '\0';

	uint32_t raw_val = strtoul(usb_str, NULL, 10);	//Most recent way of getting integers from strings
	if(raw_val > 0xFFFF) return;	//verify the value is within the range of uint16_t
	uint16_t motor_cmd = (uint16_t) raw_val;	//convert val into uint16_t (type expected by Timer 1 CCR register)
	esc_commands_t esc_command = {.cmd_cw=motor_cmd, .cmd_ccw=motor_cmd};
	motors_set_command(esc_command);
	printf("[STM32F411] motor command: %d\n", motor_cmd);
}
