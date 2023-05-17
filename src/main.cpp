#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "main.h"
#include "datatypes.h"
#include "config/confparser.h"
#include "commands.h"
#include "comm_uart.h"
#include "comm_ble.h"
#include "oled.h"
#include "buttons.h"


#include "stdio.h"
#include <string.h>

//Global variables
volatile backup_data backup;
volatile vesc_values vescData;
struct custom_hw_data_t hw_custom_data;

//Private variable

void send_hw_custom_data() 
{
		uint8_t command_test[sizeof(hw_custom_data)+1];

		command_test[0] = COMM_CUSTOM_HW_DATA;
		memcpy((void *)&command_test[1], &hw_custom_data, sizeof(hw_custom_data));
		comm_uart_send_packet(command_test, sizeof(command_test));
}

void send_app_custom_data() 
{
		uint8_t command_test[sizeof(hw_custom_data)+1];
		
		command_test[0] = COMM_CUSTOM_APP_DATA;
		memcpy((void *)&command_test[1], &hw_custom_data, sizeof(hw_custom_data));
		comm_uart_send_packet(command_test, sizeof(command_test));
}

void setup() {
    confparser_set_defaults_main_config_t((main_config_t*)(&backup.config));
    
    commands_init();
    comm_uart_init(); 
    vTaskDelay(1);
    comm_ble_init();
    oled_init();
    buttons_init();
}

void loop() {
	uint8_t command[1] = { COMM_GET_VALUES };


    while(1) {
        vTaskDelay(1000);
        comm_uart_send_packet(command, 1);
    }
    
}

