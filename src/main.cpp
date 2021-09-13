#include <Arduino.h>
#include <ArduinoJson.h>

#include "secrets.h"
#include "mqtt.h"
#include "device.h"
#include "debug.h"

#define DEVICE_ID "ESP-test" 
#define DEVICE_TYPE "ESP8266" 
#define DEVICE_STARTING_STATUS Active

#define DEBUG_ENABLED true
#define DEBUG_TICK_RATE 15

void roleChangeCallback(CapabilityRoleCode code) {
  Serial.print("[main] [roleChangeCallback] Called with code: ");
  Serial.println(code);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  setupMqtt(WIFI_SSID, WIFI_PASSWORD, MQTT_HOSTNAME, MQTT_USERNAME, MQTT_PASSWORD);
  setupDevice(DEVICE_ID, DEVICE_TYPE, DEVICE_STARTING_STATUS);

  if (DEBUG_ENABLED) {
    setupDebug(DEBUG_TICK_RATE);
  }

  Capability example = { RGB, RGB_MATRIX, String("1.0") };
  addCapability(example, roleChangeCallback);
}

void loop() {
  loopMqtt();
  // put your main code here, to run repeatedly:
}
