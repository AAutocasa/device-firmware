#include <Arduino.h>
#include <ArduinoJson.h>

#include "device.h"
#include "mqtt.h"

void activateDeviceCallback(uint8_t *data, unsigned int length) {
  Serial.println("Activate device called!");
    
  DynamicJsonDocument doc(256);
  deserializeJson(doc, data);

  const char* sensor = doc["sensor"];
  long time          = doc["time"];
  double latitude    = doc["data"][0];
  double longitude   = doc["data"][1];
}

void subscribeToActivateDevice() {
  subscribe("status-device/activate", activateDeviceCallback);
}

void deactivateDeviceCallback(uint8_t *data, unsigned int length) {
  Serial.println("Deactivate device called!");
}

void subscribeToDeactivateDevice() {
  subscribe("status-device/deactivate", deactivateDeviceCallback);
}

void roleAssignmentCallback(uint8_t *data, unsigned int length) {
  Serial.println("Role assignment called!");
}

void subscribeToRoleAssignment() {
  subscribe("status-device/capability-role", roleAssignmentCallback);
}

void setupDevice(const char *deviceID, const char *deviceType, const bool deviceStatus) {
  Serial.println(F("Print config..."));
  Serial.print(F("deviceID: "));
  Serial.println(deviceID);
  Serial.print(F("deviceType: "));
  Serial.println(deviceType);
  Serial.print(F("deviceStatus: "));
  Serial.println(deviceStatus);
  
  subscribeToActivateDevice();
  subscribeToDeactivateDevice();
  subscribeToRoleAssignment();
}
