#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "main.h"
#include "datatypes.h"
#include "commands.h"
#include "comm_uart.h"
#include "comm_ble.h"
#include "config/confparser.h"

#include "stdio.h"
#include <string.h>

//Global variables
volatile backup_data backup;

//Private variable


void app_main() {
	uint8_t command[1] = { COMM_GET_VALUES };

    confparser_set_defaults_main_config_t((main_config_t*)(&backup.config));
    
    commands_init();
    comm_uart_init(); 
    
    vTaskDelay(1);

    comm_ble_init();
    
    while(1) {
        vTaskDelay(100);
        //comm_uart_send_packet(command, 1);
    }
    
}

