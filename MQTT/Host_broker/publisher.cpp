#include <WiFi.h>
#include <PubSubClient.h>
#include "esp_sleep.h"

String msg;
#define MOTION_SENSOR_PIN 4
#define WINDOW 5

// Configura WiFi
const char* ssid = "-----";
const char* password = "-----";

// Configura EMQX
const char* mqtt_server = "-----";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected");
    } else {
      Serial.print("Connection failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(WINDOW, INPUT);

  btStop();  // Disable Bluetooth
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int motionState = digitalRead(MOTION_SENSOR_PIN);
  if (motionState == HIGH) {
    msg = "Movement";
    client.publish("PIR1", msg.c_str());
    delay(500);
  } else {
      msg = "No movement";
      client.publish("PIR1", msg.c_str());
  }

  int windowstate = digitalRead(WINDOW);
  if (windowstate == LOW) {
    msg = "Opened";
    client.publish("window1", msg.c_str());
    delay(500);
  } else {
    msg = "Closed";
    client.publish("window1", msg.c_str());
  }

}
