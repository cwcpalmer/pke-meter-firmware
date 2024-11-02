#include "bluetooth.h"
#include "debugger.h"
 
#define MAX_CONCURRENT_BLE_PERIPHERAL_CONNECTIONS   1 // This is so the hardware is a peripheral device (one connection at a time)
#define MAX_CONCURRENT_BLE_CENTRAL_CONNECTIONS      0 // If greater than 0, the hardware would be the device in charge

// First byte on write indicates which sensors to read from (max 5 sensors per characteristic)
// Response will be first byte mirroring request to indicate sensors
// Following bytes will be 4-byte each representing a float (positive or negative) starting with the smallest bit sensor first
#define SENSOR_TEMPERATURE          0b00000001
#define SENSOR_BAROMETRIC_PRESSURE  0b00000010

void start_bluetooth_advertising(char* advertisementName);
void set_up_environment_sensors_service(void);

BLEService        environmentSensorsService = BLEService("ffe34776-8d60-421a-98de-a9d137253d45");
BLECharacteristic sensorSet1Characteristic = BLECharacteristic("32824887-d3da-4744-bafc-47c9bc1d5c64");

void init_bluetooth(char* advertisementName, ble_connect_callback_t bleConnectCallback, ble_disconnect_callback_t bleDisconnectCallback) {
  // Set up hardware
  Bluefruit.begin(MAX_CONCURRENT_BLE_PERIPHERAL_CONNECTIONS, MAX_CONCURRENT_BLE_CENTRAL_CONNECTIONS);
  Bluefruit.Periph.setConnectCallback(bleConnectCallback);
  Bluefruit.Periph.setDisconnectCallback(bleDisconnectCallback);

  // Set up services
  set_up_environment_sensors_service();

  // Start advertising
  start_bluetooth_advertising(advertisementName);
}
//-------------------------------------------------------------------------------------------------
void onWrite(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  debug_ln("On Write Hit");

  if (len == 1) {
    // Determine how many bits are set in the sensors byte to size the response
    uint8_t byteCount = 1;
    for(uint8_t shift = 0; shift < 8; shift++) {
      if (data[0] & (1 << shift)) {
        byteCount += sizeof(float);
      }
    }

    uint8_t response[byteCount] = {0};
    response[0] = 0;
    uint8_t byteIndex = 1;

    // Populate each additional 4 bytes
    if (data[0] & SENSOR_TEMPERATURE) {
      response[0] |= SENSOR_TEMPERATURE; // Adding sensor flag to the first byte
      float temperatureValue = 34.72;
      // Split into big endian bytes
      memcpy(&response[byteIndex], &temperatureValue, sizeof(float));
      byteIndex += sizeof(float);
    }

    sensorSet1Characteristic.notify(response, sizeof(response));
  }
}
//-------------------------------------------------------------------------------------------------
void set_up_environment_sensors_service(void) {
  // Start the service
  environmentSensorsService.begin();

  // Configure the characteristic
  sensorSet1Characteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_WRITE_WO_RESP);
  sensorSet1Characteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  //sensorSet1Characteristic.setFixedLen(2);
  sensorSet1Characteristic.setWriteCallback(onWrite);
  sensorSet1Characteristic.begin();

  // Set initial value for temperature
  uint8_t temperatureData[1] = { 0x00 };
  sensorSet1Characteristic.write(temperatureData, sizeof(temperatureData));
}
//-------------------------------------------------------------------------------------------------
void start_bluetooth_advertising(char* advertisementName) {
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.setName(advertisementName);
  Bluefruit.ScanResponse.addName();

  // Services
  Bluefruit.Advertising.addService(environmentSensorsService);

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}