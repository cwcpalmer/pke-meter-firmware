#include "bluetooth.h"
 
#define MAX_CONCURRENT_BLE_PERIPHERAL_CONNECTIONS   1 // This is so the hardware is a peripheral device (one connection at a time)
#define MAX_CONCURRENT_BLE_CENTRAL_CONNECTIONS      0 // If greater than 0, the hardware would be the device in charge

void start_bluetooth_advertising(char* advertisementName);

void init_bluetooth(char* advertisementName, ble_connect_callback_t bleConnectCallback, ble_disconnect_callback_t bleDisconnectCallback) {
  // Set up hardware
  Bluefruit.begin(MAX_CONCURRENT_BLE_PERIPHERAL_CONNECTIONS, MAX_CONCURRENT_BLE_CENTRAL_CONNECTIONS);
  Bluefruit.Periph.setConnectCallback(bleConnectCallback);
  Bluefruit.Periph.setDisconnectCallback(bleDisconnectCallback);

  // Start advertising
  start_bluetooth_advertising(advertisementName);
}
//-------------------------------------------------------------------------------------------------
void start_bluetooth_advertising(char* advertisementName) {
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.setName(advertisementName);
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}