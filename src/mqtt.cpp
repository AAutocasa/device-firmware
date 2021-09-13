#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <map>

#include "mqtt.h"

struct MQTTInfo
{
  String mqtt_id;
  String mqtt_username;
  String mqtt_password;
};

WiFiClient espClient;
MQTTInfo mqtt_info;

PubSubClient *client;

std::map<String, Callback> callbacks;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived. Topíc: [");
  Serial.print(topic);
  Serial.print("]. Message: ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String topicString = String(topic);
  Callback callback = callbacks[topicString];

  if (callback)
  {
    callback(payload, length);
  }
}

int connect()
{
  return client->connect(mqtt_info.mqtt_id.c_str(), mqtt_info.mqtt_username.c_str(), mqtt_info.mqtt_password.c_str());
}

void setupMqtt(const char *ssid, const char *wifi_password, const char *mqtt_hostname, const char *mqtt_username, const char *mqtt_password)
{
  WiFi.begin(ssid, wifi_password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  { // Connect to WiFi.
    Serial.println("Couldn't connect to WiFi.");
    delay(10000);
    WiFi.begin(ssid, wifi_password);
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);

  mqtt_info.mqtt_id = clientId;
  mqtt_info.mqtt_username = mqtt_username;
  mqtt_info.mqtt_password = mqtt_password;

  client = new PubSubClient(mqtt_hostname, 1883, callback, espClient);
  connect();

  Serial.println("Connected to client");

  client->setCallback(callback);
  client->subscribe("MQTTTest");

  Serial.println("Callback set!");
}

void reconnect()
{
  while (!client->connected())
  {
    Serial.print("Attempting MQTT connection...");

    if (connect())
    {
      Serial.println("Connected!");
    }
    else
    {
      Serial.print("Failed, rc=");
      Serial.print(client->state());
      Serial.println("Trying again in in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loopMqtt()
{
  if (!client->connected())
  {
    reconnect();
  }
  client->loop();
}

void subscribe(String topic, Callback callback)
{
  client->subscribe(topic.c_str());
  callbacks[topic] = callback;
}