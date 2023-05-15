#ifndef __COMM_UART_H__
#define __COMM_UART_H__

void comm_uart_init(void);
void comm_uart_send_packet(unsigned char *data, unsigned int len);

#endif /* __COMM_UART_H__ */