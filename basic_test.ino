#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif

#include <Adafruit_BMP280.h> // for onboard temperature and barometric pressure
#include "debugger.h"
#include "neopixel.h"
#include "bluetooth.h"

Adafruit_BMP280 sensor_onboard_temp_barometric;

float reading_onboard_temp;

bool isConnected = false;

//-------------------------------------------------------------------------------------------------
void onBleConnect(uint16_t connectionHandle) {
  debug_ln("Connected Bluetooth");
  isConnected = true;
  set_neopixel_color(NEOPIXEL_COLOR_BLUE);
}
//-------------------------------------------------------------------------------------------------
void onBleDisconnect(uint16_t connectionHandle, uint8_t disconnectReason) {
  debug_ln("Disconnected Bluetooth");
  isConnected = false;
  set_neopixel_color(NEOPIXEL_COLOR_YELLOW);
}
//-------------------------------------------------------------------------------------------------
void setup() {
  init_debugger();
  init_neopixel();
  init_bluetooth((char*)"PKE Meter: V", onBleConnect, onBleDisconnect);
  sensor_onboard_temp_barometric.begin();
}
//-------------------------------------------------------------------------------------------------
void loop() {
  if(isConnected) {
    reading_onboard_temp = ((sensor_onboard_temp_barometric.readTemperature() * 9) / 5) + 32;
    debug_ln(String("Temperature: ") + String(reading_onboard_temp) + String(" F"));
  }

  delay(5000);
}
