#include <WiFi.h>
#include <PubSubClient.h>

// Configurazione WiFi
const char* ssid = "-----";       // insert your WiFi ID
const char* password = "-----";   // insert your WiFi password

// Configurazione Broker MQTT
const char* mqtt_server = "------";      // IP of the broker MQTT
const int mqtt_port = 1883;              
const char* mqtt_topic = "test/topic";

WiFiClient espClient;
PubSubClient client(espClient);

// called when a messaged is received
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Messagge on topic: ");
  Serial.println(topic);
  Serial.print("Content: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup_wifi() {
  delay(10);
  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected WiFi!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to the broker MQTT...");
    if (client.connect("ESP32Subscriber")) {
      Serial.println("Connected to the broker MQTT!");
      client.subscribe(mqtt_topic);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic);
    } else {
      Serial.print("Connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 5 s...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
