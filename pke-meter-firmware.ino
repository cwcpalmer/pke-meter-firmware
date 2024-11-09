#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif

#include <Adafruit_BMP280.h> // for onboard temperature and barometric pressure
#include "debugger.h"
#include "neopixel.h"
#include "bluetooth.h"

// CONFIG VALUES
#define BLUETOOTH_NAME  "PKE Meter: Matty"

Adafruit_BMP280 sensor_onboard_temp_barometric;

float reading_onboard_temp;

bool isConnected = false;

static SensorReadings sensorSet1Callbacks;

void updateBattery();

//-------------------------------------------------------------------------------------------------
void onBleConnect(uint16_t connectionHandle) {
  debug_ln("Connected Bluetooth");
  isConnected = true;
  set_neopixel_color(NEOPIXEL_COLOR_BLUE);
  updateBattery();
}
//-------------------------------------------------------------------------------------------------
void onBleDisconnect(uint16_t connectionHandle, uint8_t disconnectReason) {
  debug_ln("Disconnected Bluetooth");
  isConnected = false;
  set_neopixel_color(NEOPIXEL_COLOR_YELLOW);
}
//-------------------------------------------------------------------------------------------------
static float updateTemperature(void) {
  return sensor_onboard_temp_barometric.readTemperature();
} 
//-------------------------------------------------------------------------------------------------
void updateBattery() {
  int calculatedLevel = 0;

  float measuredvbat = analogRead(A6);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.6;  // Multiply by 3.6V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  // Convert to percentage
  calculatedLevel = (int)((measuredvbat - 3.2) * 100); // 3.2 is the shutoff point, 4.2 is the max (4.2-3.2=1 so we don't divide)

  if (calculatedLevel > 100) {
    calculatedLevel = 100;
  } else if (calculatedLevel < 0) {
    calculatedLevel = 0;
  }

  update_battery_level(calculatedLevel);
  debug_ln(String("Battery: ") + String(calculatedLevel) + String("%"));
}
//-------------------------------------------------------------------------------------------------
void setup() {
  init_debugger();
  init_neopixel();

  // Configure callbacks for first set of sensors (characteristic)
  sensorSet1Callbacks.sensor1 = updateTemperature;

  init_bluetooth((char*)BLUETOOTH_NAME, onBleConnect, onBleDisconnect, sensorSet1Callbacks);
  sensor_onboard_temp_barometric.begin();
}
//-------------------------------------------------------------------------------------------------
void loop() {
  if(isConnected) {
    reading_onboard_temp = sensor_onboard_temp_barometric.readTemperature();
    debug_ln(String("Temperature: ") + String(reading_onboard_temp) + String(" C"));
    updateBattery();
  }

  delay(30000);
}
