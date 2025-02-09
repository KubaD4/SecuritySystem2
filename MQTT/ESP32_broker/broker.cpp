#include <WiFi.h>
#include "EmbeddedMqttBroker.h"
#include <PubSubClient.h>

using namespace mqttBrokerName;

// WiFi credentials
const char *ssid = "------------";
const char *password = "-------------";

/******************* MQTT broker ********************/
uint16_t mqttPort = 1883;
MqttBroker broker(mqttPort);

// MQTT client object
WiFiClient espClient;
PubSubClient client(espClient);

// Callback function to handle received messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Content: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Function to connect the MQTT client to the broker
void reconnectClient() {
  while (!client.connected()) {
    Serial.print("Connecting as MQTT client to ");
    Serial.print(WiFi.localIP());
    Serial.print(":");
    Serial.println(mqttPort);

    if (client.connect("Broker_Client")) {
      Serial.println("Connected as MQTT client!");
      client.subscribe("test/topic");
    } else {
      Serial.print("Connection failed. State=");
      Serial.println(client.state());
      delay(2000); // Retry connection after 2 seconds
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Start WiFi
  Serial.println("\n\nSimple MQTT Broker with client");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");

  // Start the MQTT broker
  broker.setMaxNumClients(9); // Set the maximum number of clients
  broker.startBroker();
  Serial.println("MQTT Broker started");

  // Print connection information
  Serial.print("Use this IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("and this port: ");
  Serial.println(mqttPort);

/*
  // Configure the MQTT client
  client.setServer("------", mqttPort); // The broker is local
  client.setCallback(callback); // Register the callback
*/
}

void loop() {
  // Check MQTT client connection
/*  if (!client.connected()) {
    reconnectClient();
  }
  client.loop(); // Handle the MQTT client loop
*/
}
