#ifndef __bluetooth__
#define __bluetooth__

#include <bluefruit.h>
#include <arduino.h>

typedef float (*sensor_update_callback_t) (void);

struct SensorReadings {
  sensor_update_callback_t sensor1;
  sensor_update_callback_t sensor2;
  sensor_update_callback_t sensor3;
  sensor_update_callback_t sensor4;
  sensor_update_callback_t sensor5;
};

void init_bluetooth(char*, ble_connect_callback_t, ble_disconnect_callback_t, SensorReadings);
void update_battery_level(int batteryPercentage);

#endif