#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include "Wire.h"
#include "SPI.h"
#include "datatypes.h"
#include "conf_general.h"
#include "HardwareSerial.h"
#include "oled/OLEDDisplayUi.h"

struct custom_hw_data_t {
	uint16_t light_on;
};

extern struct custom_hw_data_t hw_custom_data;

extern volatile backup_data backup;
extern volatile vesc_values vescData;
extern OLEDDisplayUi ui;

void send_hw_custom_data();
void send_app_custom_data();
#endif /* MAIN */