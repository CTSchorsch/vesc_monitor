#ifndef __CONF_GENERAL_H__
#define __CONF_GENERAL_H__

/* 
    Do some custom defines here
    defaults are in config/conf_default.h
*/

//UART to VESC 
#define CONF_VESC_UART_NUM 1
#define CONF_VESC_UART_RX_PIN 45
#define CONF_VESC_UART_TX_PIN 42

#define CONF_OLED_BAT_FULL  54.0
#define CONF_OLED_BAT_EMPTY 44.0

#define CONF_BUTTON_MENU_PIN    0

//special ones
#define ID_GEORG    14
#define ID_THOMAS   15

#include "config/conf_default.h"

#endif /* __CONF_GENERAL_H__ */