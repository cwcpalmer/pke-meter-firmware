#ifndef __bluetooth__
#define __bluetooth__

#include <bluefruit.h>
#include <arduino.h>

void init_bluetooth(char*, ble_connect_callback_t, ble_disconnect_callback_t);

#endif