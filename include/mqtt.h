#ifndef __MQTT_H__
#define __MQTT_H__

#define MQTTCallback std::function<void(uint8_t *, unsigned int)>

void setupMqtt(const char *ssid, const char *wifi_password, const char *mqtt_hostname, const char *mqtt_username, const char *mqtt_password);
void loopMqtt();

void subscribe(String topic, MQTTCallback callback);
void publish(String topic, String message);

#endif