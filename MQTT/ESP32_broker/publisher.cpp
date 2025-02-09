#include <WiFi.h>
#include <PubSubClient.h>

String msg = "";
#define MOTION_SENSOR_PIN 4 
// WiFi credentials
const char* ssid = "-------";
const char* password = "----------";

// MQTT Broker IP (ESP32 Broker)
const char* mqtt_server = "-----------";
const int mqtt_port = 1883;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("connected!");
      client.subscribe("test/topic");
    } else {
      Serial.print("failed, state=");
      Serial.println(client.state()); // Print detailed error state
      delay(2000); // Wait before retrying
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Configure the sensor pin as input
  pinMode(MOTION_SENSOR_PIN, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int motionState = digitalRead(MOTION_SENSOR_PIN);
  // Publish a message every 5 seconds
    if (motionState == HIGH){
      msg = "Movement";
      client.publish("test/topic", msg.c_str());
      delay(5000);
    } 
}
