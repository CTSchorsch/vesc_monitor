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

#include "main.h"
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

	packet_id = (COMM_PACKET_ID) data[0];
	data++;
	len--;

	send_func = reply_func;

	// Avoid calling invalid function pointer if it is null.
	if (!reply_func ) {
		reply_func = send_func_dummy;
	}

	int32_t ind = 0;	
	switch (packet_id) {
		case COMM_GET_VALUES:
                vescData.tempMosfet = buffer_get_float16(data, 10.0, &ind);
                vescData.tempMotor = buffer_get_float16(data, 10.0, &ind);
                vescData.avgMotorCurrent = buffer_get_float32(data, 100.0, &ind);
                vescData.avgInputCurrent = buffer_get_float32(data, 100.0, &ind);
                ind += 8; // Skip the next 8 bytes
                vescData.dutyCycleNow = buffer_get_float16(data, 1000.0, &ind);
                vescData.rpm = buffer_get_int32(data, &ind);
                vescData.inpVoltage = buffer_get_float16(data, 10.0, &ind);
                vescData.ampHours = buffer_get_float32(data, 10000.0, &ind);
                vescData.ampHoursCharged = buffer_get_float32(data, 10000.0, &ind);
                ind += 8; // Skip the next 8 bytes
                vescData.tachometer = buffer_get_int32(data, &ind);
                vescData.tachometerAbs = buffer_get_int32(data, &ind);
                vescData.error = data[ind++];
                ind += 4;
                vescData.id = data[ind++];
                vescData.tempLED = buffer_get_float16(data, 10.0, &ind);
                vescData.tempMisc1 = buffer_get_float16(data, 10.0, &ind);
			break;

		default:
			printf("PACKET ID %d\n",packet_id);
			break;
	}
}

void commands_send_packet(unsigned char *data, unsigned int len) {
	if (send_func) {
		send_func(data, len);
	}
}


