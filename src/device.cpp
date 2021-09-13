#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ticker.h>

#include <map>
#include <vector>

#include "device.h"
#include "mqtt.h"


String _deviceID;
String _deviceType;
DeviceStatus _deviceStatus;
std::vector<Capability> capabilities;

Ticker heartbeatTicker;

std::map<CapabilityCode, RoleChangeCallback> capabilitiesCallbacks;

// Device functions
void activateDevice() {
  _deviceStatus = Active;
  Serial.println("[device.cpp] [activateDevice] Device status is now Active!");
}

void deactivateDevice() {
  _deviceStatus = Inactive;
  Serial.println("[device.cpp] [deactivateDevice] Device status is now Inactive!");
}

void assignRole(CapabilityCode code, CapabilityRoleCode activeRole) {
  Serial.println("[device.cpp] [assignRole] assignRole called!");
    
  Capability *matchingCapability = NULL;

  for (auto &capability: capabilities) {
    if (capability.code == code) {
      matchingCapability = &capability;
      break;
    }
  }

  if (matchingCapability) {
    matchingCapability->activeRole = activeRole;
    capabilitiesCallbacks[code](activeRole);
  }  
}

// MQTT-related
bool isMessageToDevice(DynamicJsonDocument doc) {
  const char* deviceId = doc["deviceId"];

  return (deviceId && String(deviceId) == _deviceID);
}

void activateDeviceCallback(uint8_t *data, unsigned int length) {
  Serial.println("[device.cpp] [activateDeviceCallback] Activate device called!");
    
  DynamicJsonDocument doc(64);
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
  
  DynamicJsonDocument doc(64);
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
  
  DynamicJsonDocument doc(128);
  deserializeJson(doc, data);

  CapabilityCode code = doc["code"];
  CapabilityRoleCode roleCode = doc["roleCode"];
  
  if (isMessageToDevice(doc)) {
    assignRole(code, roleCode);
  }
}

void subscribeToRoleAssignment() {
  subscribe("status-device/capability-role", roleAssignmentCallback);
}

void sendHeartbeat() {
  DynamicJsonDocument heartbeatDoc(512);
  
  Serial.println(F("[device.cpp] [sendHeartbeat] Started..."));
  heartbeatDoc["id"] = _deviceID;
  heartbeatDoc["type"] = _deviceType;
  heartbeatDoc["status"] = _deviceStatus;

  unsigned int size = capabilities.size();
  for (unsigned int i = 0; i < size; i++) {
    Capability *capability = &capabilities[i];
    heartbeatDoc["capabilities"][i]["code"] = capability->code;
    heartbeatDoc["capabilities"][i]["activeRole"] = capability->activeRole;
    heartbeatDoc["capabilities"][i]["version"] = capability->version;
  }

  String output;
  serializeJson(heartbeatDoc, output);

  publish("status/heartbeat", output);
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

  heartbeatTicker.attach(5, sendHeartbeat);
  
  subscribeToActivateDevice();
  subscribeToDeactivateDevice();
  subscribeToRoleAssignment();
}

void addCapability(Capability capability, RoleChangeCallback callback) {
  capabilities.push_back(capability);
  capabilitiesCallbacks[capability.code] = callback;
}