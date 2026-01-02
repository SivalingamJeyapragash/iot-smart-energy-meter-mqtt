#include <WiFi.h>
#include <PubSubClient.h>

// ---------------- WIFI CONFIG (PLACEHOLDERS) ----------------
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------------- MQTT CONFIG (PLACEHOLDERS) ----------------
const char* mqtt_server = "YOUR_MQTT_BROKER_IP";
const int mqtt_port = 1883;
const char* mqtt_client_id = "esp32_energy_meter";

// ---------------- MQTT TOPICS ----------------
const char* topic_voltage = "energy/house_01/voltage";
const char* topic_current = "energy/house_01/current";
const char* topic_power   = "energy/house_01/power";
const char* topic_units   = "energy/house_01/units";
const char* topic_relay   = "energy/house_01/relay_control";

// ---------------- PIN DEFINITIONS ----------------
#define VOLTAGE_PIN 34      // ZMPT101B
#define CURRENT_PIN 35      // ACS712
#define RELAY_PIN   26

WiFiClient espClient;
PubSubClient client(espClient);

// ---------------- ENERGY VARIABLES ----------------
float voltage = 0.0;
float current = 0.0;
float power = 0.0;
float energy_units = 0.0;
unsigned long lastMillis = 0;

// ---------------- WIFI CONNECT ----------------
void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

// ---------------- MQTT CALLBACK ----------------
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == topic_relay) {
    if (message == "ON") {
      digitalWrite(RELAY_PIN, HIGH);
    } else if (message == "OFF") {
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}

// ---------------- MQTT CONNECT ----------------
void reconnectMQTT() {
  while (!client.connected()) {
    if (client.connect(mqtt_client_id)) {
      client.subscribe(topic_relay);
    } else {
      delay(2000);
    }
  }
}

// ---------------- SENSOR READ FUNCTIONS ----------------
float readVoltage() {
  int raw = analogRead(VOLTAGE_PIN);
  return raw * (3.3 / 4095.0) * 100;  // Calibration needed
}

float readCurrent() {
  int raw = analogRead(CURRENT_PIN);
  return (raw - 2048) * (3.3 / 4095.0); // Calibration needed
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  setupWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  lastMillis = millis();
}

// ---------------- LOOP ----------------
void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  voltage = readVoltage();
  current = readCurrent();
  power = voltage * current;

  unsigned long now = millis();
  float hours = (now - lastMillis) / 3600000.0;
  energy_units += (power * hours) / 1000.0; // kWh
  lastMillis = now;

  client.publish(topic_voltage, String(voltage).c_str());
  client.publish(topic_current, String(current).c_str());
  client.publish(topic_power, String(power).c_str());
  client.publish(topic_units, String(energy_units).c_str());

  delay(5000);
}
