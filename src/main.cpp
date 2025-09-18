#include <WiFi.h>
#include "mqtt_server.h"
#include "mqtt_client.h"


// Comment this out if not using Wokwi simulator
#define WOKWI

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// ====== WIFI ======
#ifdef WOKWI
// Wokwi simulated WiFi AP is open (no password) and uses channel 6
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";
const int WIFI_CHANNEL = 6;
#else
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASSWORD";
#endif

// MQTT Server and Client instances
MqttServer mqttServer;
MqttClient mqttClient;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  delay(200);

  Serial.println("\r\nESP32 MQTT demo");

  // WIFI connect
  WiFi.mode(WIFI_STA);
#ifdef WOKWI
  WiFi.begin(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);
#else
  WiFi.begin(WIFI_SSID, WIFI_PASS);
#endif
  Serial.printf("Connecting to %s", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.printf("\r\nWiFi connected, IP: %s\r\n", WiFi.localIP().toString().c_str());
#ifdef WOKWI
  Serial.println("Note: Running in Wokwi.");
#endif

  // Initialize MQTT server and client
  mqttServer.begin();
  mqttClient.begin();
}

void loop() {
  // Handle MQTT server and client traffic
  mqttServer.loop();
  mqttClient.loop();
  delay(1);
}