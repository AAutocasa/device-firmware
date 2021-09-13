#ifndef __MQTT_H__
#define __MQTT_H__

#define Callback std::function<void(uint8_t *, unsigned int)>

void setupMqtt(const char *ssid, const char *wifi_password, const char *mqtt_hostname, const char *mqtt_username, const char *mqtt_password);
void loopMqtt();

void subscribe(String topic, Callback callback);

#endif