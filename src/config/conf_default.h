#ifndef __CONF_DEFAULT_H__
#define __CONF_DEFAULT_H__

//UART to VESC
#ifndef CONF_VESC_BAUD_RATE
#define CONF_VESC_BAUD_RATE 115200
#endif

#ifndef CONF_VESC_UART_NUM
#define CONF_VESC_UART_NUM 1
#endif

// Bluetooth 
#ifndef CONF_BLE_MODE
#define CONF_BLE_MODE 0
#endif

#ifndef CONF_BLE_NAME
#define CONF_BLE_NAME "VescMon"
#endif

#ifndef CONF_BLE_PIN
#define CONF_BLE_PIN 123456
#endif

#endif /* __CONF_DEFAULT_H__*/