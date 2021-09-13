#include <Arduino.h>
#include <ArduinoJson.h>

#include "device.h"
#include "mqtt.h"

String _deviceID;
String _deviceType;
DeviceStatus _deviceStatus;

// Device functions
void activateDevice() {
  _deviceStatus = Active;
  Serial.println("[device.cpp] [activateDevice] Device status is now Active!");
}

void deactivateDevice() {
  _deviceStatus = Inactive;
  Serial.println("[device.cpp] [deactivateDevice] Device status is now Inactive!");
}

void assignRole() {
  Serial.println("[device.cpp] [assignRole] assignRole called!");
}

// MQTT-related
bool isMessageToDevice(DynamicJsonDocument doc) {
  const char* deviceId = doc["deviceId"];

  return (deviceId && String(deviceId) == _deviceID);
}

void activateDeviceCallback(uint8_t *data, unsigned int length) {
  Serial.println("[device.cpp] [activateDeviceCallback] Activate device called!");
    
  DynamicJsonDocument doc(256);
  deserializeJson(doc, data);
 
  if (isMessageToDevice(doc)) {
    activateDevice();
  }
}

void subscribeToActivateDevice() {
  subscribe("status-device/activate", activateDeviceCallback);
}

void deactivateDeviceCallback(uint8_t *data, unsigned int length) {
  Serial.println("[device.cpp] [deactivateDeviceCallback] Deactivate device called!");
  
  DynamicJsonDocument doc(256);
  deserializeJson(doc, data);

  if (isMessageToDevice(doc)) {
    deactivateDevice();
  }
}

void subscribeToDeactivateDevice() {
  subscribe("status-device/deactivate", deactivateDeviceCallback);
}

void roleAssignmentCallback(uint8_t *data, unsigned int length) {
  Serial.println("[device.cpp] [roleAssignmentCallback] Role assignment called!");
  
  DynamicJsonDocument doc(256);
  deserializeJson(doc, data);
  
  if (isMessageToDevice(doc)) {
    assignRole();
  }
}

void subscribeToRoleAssignment() {
  subscribe("status-device/capability-role", roleAssignmentCallback);
}

void setupDevice(const char *deviceID, const char *deviceType, const DeviceStatus deviceStatus) {
  Serial.println(F("[device.cpp] [setupDevice] Print config..."));
  Serial.print(F("[device.cpp] [setupDevice] deviceID: "));
  Serial.println(deviceID);
  Serial.print(F("[device.cpp] [setupDevice] deviceType: "));
  Serial.println(deviceType);
  Serial.print(F("[device.cpp] [setupDevice] deviceStatus: "));
  Serial.println(deviceStatus);

  _deviceID = String(deviceID);
  _deviceType = String(deviceType);
  _deviceStatus = deviceStatus;
  
  subscribeToActivateDevice();
  subscribeToDeactivateDevice();
  subscribeToRoleAssignment();
}
