#include <Arduino.h>
#include <ArduinoJson.h>

#include "secrets.h"
#include "mqtt.h"
#include "device.h"

#define DEVICE_ID "ESP-test" 
#define DEVICE_TYPE "ESP8266" 
#define DEVICE_STARTING_STATUS Active

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  setupMqtt(WIFI_SSID, WIFI_PASSWORD, MQTT_HOSTNAME, MQTT_USERNAME, MQTT_PASSWORD);
  setupDevice(DEVICE_ID, DEVICE_TYPE, DEVICE_STARTING_STATUS);
}

void loop() {
  loopMqtt();
  // put your main code here, to run repeatedly:
}
