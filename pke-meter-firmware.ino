#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif

#include <Adafruit_BMP280.h> // for onboard temperature and barometric pressure
#include <Wire.h>
#include "Adafruit_MCP9808.h" // for more precise temperature sensor
#include "debugger.h"
#include "neopixel.h"
#include "bluetooth.h"

// CONFIG VALUES
#define BLUETOOTH_NAME                "PKE Meter: John"
#define SENSOR_ADDON_TEMP_I2C_ADDRESS 0x18
#define SENSOR_ADDON_TEMP_RESOLUTION  3 // 0-3 where higher is more precise at cost of sample time

Adafruit_BMP280 sensor_onboard_temp_barometric;
Adafruit_MCP9808 sensor_addon_temp = Adafruit_MCP9808();

float reading_onboard_temp;
float reading_addon_temp;

bool isConnected = false;

static SensorReadings sensorSet1Callbacks;

void updateBattery();

//-------------------------------------------------------------------------------------------------
void onBleConnect(uint16_t connectionHandle) {
  debug_ln("Connected Bluetooth");
  sensor_addon_temp.wake();
  isConnected = true;
  set_neopixel_color(NEOPIXEL_COLOR_BLUE);
  updateBattery();
}
//-------------------------------------------------------------------------------------------------
void onBleDisconnect(uint16_t connectionHandle, uint8_t disconnectReason) {
  debug_ln("Disconnected Bluetooth");
  isConnected = false;
  sensor_addon_temp.shutdown_wake(1);
  set_neopixel_color(NEOPIXEL_COLOR_YELLOW);
}
//-------------------------------------------------------------------------------------------------
static float updateTemperature1(void) {
  return sensor_onboard_temp_barometric.readTemperature();
} 
//-------------------------------------------------------------------------------------------------
static float updateTemperature2(void) {
  return sensor_addon_temp.readTempC();
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

  // Configure addon temperature sensor board
  if (!sensor_addon_temp.begin(SENSOR_ADDON_TEMP_I2C_ADDRESS)) {
    while(1) {
      debug_ln(String("Could not connect to addon temp sensor"));
      delay(1000);
    }
  }

  sensor_addon_temp.setResolution(SENSOR_ADDON_TEMP_RESOLUTION);
  sensor_addon_temp.shutdown_wake(1);

  // Configure callbacks for first set of sensors (characteristic)
  sensorSet1Callbacks.sensor1 = updateTemperature1;
  sensorSet1Callbacks.sensor2 = updateTemperature2;

  init_bluetooth((char*)BLUETOOTH_NAME, onBleConnect, onBleDisconnect, sensorSet1Callbacks);
  sensor_onboard_temp_barometric.begin();
}
//-------------------------------------------------------------------------------------------------
void loop() {
#if __DEBUGGER_ON__
    // On-board Temperature
    reading_onboard_temp = sensor_onboard_temp_barometric.readTemperature();
    debug_ln(String("Onboard Temperature: ") + String(reading_onboard_temp) + String(" C"));

    // Add-on Temperature
    sensor_addon_temp.wake();
    reading_addon_temp = sensor_addon_temp.readTempC();
    debug_ln(String("Addon Temperature: ") + String(reading_addon_temp) + String(" C"));
    //sensor_addon_temp.shutdown_wake(1);
#endif

  if(isConnected) {
    updateBattery();
  }

  delay(30000);
}
