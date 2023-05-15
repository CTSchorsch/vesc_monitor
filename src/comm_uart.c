#include "main.h"
#include "commands.h"
#include "comm_uart.h"
#include "packet.h"
#include "driver/uart.h"


static PACKET_STATE_t packet_state;


static void rx_task(void *arg) 
{
    printf("Startinge UART RX Thread\n");
    for(;;) {
        uint8_t buf[1];
        uart_read_bytes(CONF_VESC_UART_NUM, buf, 1, portMAX_DELAY);
        packet_process_byte(buf[0], &packet_state);
    }

}

//callback after unpacking data
static void process_packet(unsigned char *data, unsigned int len) 
{
    //process received packet and send answer with given callback
    commands_process_packet(data, len, comm_uart_send_packet);
}

//callback for sending packet over line
static void send_packet_raw(unsigned char *buffer, unsigned int len)
{
    uart_write_bytes(CONF_VESC_UART_NUM, buffer, len);
}

void comm_uart_init(void) 
{
    uart_config_t uart_config;
    uart_config.baud_rate  = CONF_VESC_BAUD_RATE;
    uart_config.data_bits  = UART_DATA_8_BITS;
    uart_config.parity     = UART_PARITY_DISABLE;
    uart_config.stop_bits  = UART_STOP_BITS_1;
    uart_config.flow_ctrl   = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;
    uart_config.rx_flow_ctrl_thresh = 0;

    uart_driver_install(CONF_VESC_UART_NUM, 512, 512, 0, 0, 0);
    uart_param_config( CONF_VESC_UART_NUM, &uart_config);
    uart_set_pin(CONF_VESC_UART_NUM, 42, 45, -1, -1);
    packet_init(send_packet_raw, process_packet, &packet_state);
    xTaskCreatePinnedToCore(rx_task, "uart_rx", 3072, NULL, 8, NULL, tskNO_AFFINITY);
}

//build packet and call raw send function
void comm_uart_send_packet(unsigned char *data, unsigned int len)
{
    packet_send_packet(data,len, &packet_state);
}