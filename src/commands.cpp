/*
	Copyright 2022 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "commands.h"
#include "datatypes.h"
#include "packet.h"
#include "buffer.h"
#include "crc.h"
#include "comm_ble.h"

// Settings
#define PRINT_BUFFER_SIZE	400

// For double precision literals
#define D(x) 						((double)x##L)

// Private variables
static SemaphoreHandle_t print_mutex;
static bool init_done = false;


// Function pointers
static void(* volatile send_func)(unsigned char *data, unsigned int len) = 0;

static void send_func_dummy(unsigned char *data, unsigned int len) {
	(void)data; (void)len;
}

void commands_init(void) {
	print_mutex = xSemaphoreCreateMutex();
	init_done = true;
}

void commands_process_packet(unsigned char *data, unsigned int len,
		void(*reply_func)(unsigned char *data, unsigned int len)) {
	if (!len) {
		return;
	}

	//forward
	if (comm_ble_is_connected()) {
		comm_ble_send_packet(data, len);
	}

	//local 
	COMM_PACKET_ID packet_id;

	packet_id = data[0];
	data++;
	len--;

	send_func = reply_func;

	// Avoid calling invalid function pointer if it is null.
	if (!reply_func ) {
		reply_func = send_func_dummy;
	}


	switch (packet_id) {
		case COMM_GET_VALUES:
			printf("GET Values\n");
			break;

		default:
			break;
	}
}

void commands_send_packet(unsigned char *data, unsigned int len) {
	if (send_func) {
		send_func(data, len);
	}
}

int commands_printf(const char* format, ...) {
	if (!init_done) {
		return 0;
	}

	xSemaphoreTake(print_mutex, portMAX_DELAY);

	va_list arg;
	va_start (arg, format);
	int len;

	char *print_buffer = malloc(PRINT_BUFFER_SIZE);

	print_buffer[0] = COMM_PRINT;
	len = vsnprintf(print_buffer + 1, (PRINT_BUFFER_SIZE - 1), format, arg);
	va_end (arg);

	int len_to_print = (len < (PRINT_BUFFER_SIZE - 1)) ? len + 1 : PRINT_BUFFER_SIZE;

	if(len > 0) {
		commands_send_packet((unsigned char*)print_buffer, len_to_print);
	}

	free(print_buffer);
	xSemaphoreGive(print_mutex);

	return len_to_print - 1;
}

