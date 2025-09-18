#include "mqtt_client.h"

// Static constants
const uint32_t MqttClient::ledTogglePeriodMs;

// Static instance pointer
MqttClient* MqttClient::instance = nullptr;

MqttClient::MqttClient() : mqtt(wifiClient) {
    instance = this;
    helloAtMs = 1000;
    helloSent = false;
    nextMqttLedToggleMs = 0;
    nextLedStateOn = true;
}

void MqttClient::begin() {
    // Configure MQTT broker (using a public test broker)
    mqtt.setServer("mqtt-dashboard.com", 1883);
    mqtt.setCallback(mqttCallback);
    
    // Connect to MQTT broker
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (mqtt.connect(clientId.c_str())) {
        Serial.println("MQTT client connected to broker");
        
        // Subscribe to response topics
        mqtt.subscribe("x7k9m2q8/hello/response");
        mqtt.subscribe("x7k9m2q8/led/response");
        
        // Schedule a one-time self-hello after ~5 seconds
        helloAtMs = millis() + 5000;
    } else {
        Serial.print("MQTT client connection failed, rc=");
        Serial.println(mqtt.state());
    }
}

void MqttClient::loop() {
    if (!mqtt.connected()) {
        // Reconnect if disconnected
        String clientId = "ESP32Client-" + String(random(0xffff), HEX);
        if (mqtt.connect(clientId.c_str())) {
            mqtt.subscribe("x7k9m2q8/hello/response");
            mqtt.subscribe("x7k9m2q8/led/response");
        }
    }
    
    // Send periodic MQTT message to toggle LED state
    if ((long)(millis() - nextMqttLedToggleMs) >= 0) {
        const char* body = nextLedStateOn ? "on" : "off";
        Serial.printf("[Client] led: Publishing '%s' to x7k9m2q8/led\r\n", body);
        mqtt.publish("x7k9m2q8/led", body);
        nextLedStateOn = !nextLedStateOn;
        nextMqttLedToggleMs += ledTogglePeriodMs;
    }

    // Send a one-time self MQTT message to /hello after a few seconds
    if (!helloSent && millis() >= helloAtMs) {
        Serial.printf("[Client] hello: Publishing 'hello' to x7k9m2q8/hello\r\n");
        mqtt.publish("x7k9m2q8/hello", "hello");
        helloSent = true;
    }

    // Handle inbound MQTT client traffic
    mqtt.loop();
}

void MqttClient::mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Convert payload to string
    String message;
    message.reserve(length + 1);
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.printf("[Client] Got MQTT response on topic '%s': '%s'\r\n", topic, message.c_str());
}