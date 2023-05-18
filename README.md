# vesc_monitor
Use a Heltec Wifi Kit module as display and BLE bridge for VESC devices.

It connects to a VESC device via UART instead of CAN and is a simple BLE bridge without own ID or commands to send to.
In addition it sends a COMM_GET_VALUES command to the VESC device for local use to feed the display with informations.
It can send custom_app_data and custom_hw_data to the VESC device

## Hardware
https://heltec.org/project/wifi-kit-32-v3/

## Software
Its a Platformio/Arduino Project. Should be easy to import to vscode/platformio

