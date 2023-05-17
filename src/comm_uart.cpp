#include "main.h"
#include "commands.h"
#include "comm_uart.h"
#include "packet.h"
#include "HardwareSerial.h"

HardwareSerial vescUart(1);


static PACKET_STATE_t packet_state;


static void rx_task(void *arg) 
{
    printf("Startinge UART RX Thread\n");
    for(;;) {
        uint8_t buf[1];
        if (vescUart.available()) {
            vescUart.readBytes(buf,1);
        }
        vTaskDelay(1);
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
    vescUart.write(buffer, len);

}

void comm_uart_init(void) 
{
    vescUart.begin(115200,SERIAL_8N1, CONF_VESC_UART_RX_PIN,CONF_VESC_UART_TX_PIN);
    while(!vescUart);
    packet_init(send_packet_raw, process_packet, &packet_state);
    xTaskCreatePinnedToCore(rx_task, "uart_rx", 3072, NULL, 8, NULL, tskNO_AFFINITY);
}

//build packet and call raw send function
void comm_uart_send_packet(unsigned char *data, unsigned int len)
{
   packet_send_packet(data,len, &packet_state);
}