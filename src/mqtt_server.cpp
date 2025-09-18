#include "mqtt_server.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Static instance pointer
MqttServer* MqttServer::instance = nullptr;

MqttServer::MqttServer() : mqtt(wifiClient) {
    instance = this;
}

void MqttServer::begin() {
    // Configure MQTT broker (using a public test broker)
    mqtt.setServer("mqtt-dashboard.com", 1883);
    mqtt.setCallback(mqttCallback);
    
    // Connect to MQTT broker
    String clientId = "ESP32Server-" + String(random(0xffff), HEX);
    if (mqtt.connect(clientId.c_str())) {
        Serial.println("MQTT server connected to broker");
        
        // Subscribe to topics
        mqtt.subscribe("x7k9m2q8/hello");
        mqtt.subscribe("x7k9m2q8/led");
        
        Serial.println("MQTT server started, subscribed to x7k9m2q8/hello and x7k9m2q8/led");
    } else {
        Serial.print("MQTT server connection failed, rc=");
        Serial.println(mqtt.state());
    }
}

void MqttServer::loop() {
    if (!mqtt.connected()) {
        // Reconnect if disconnected
        String clientId = "ESP32Server-" + String(random(0xffff), HEX);
        if (mqtt.connect(clientId.c_str())) {
            mqtt.subscribe("x7k9m2q8/hello");
            mqtt.subscribe("x7k9m2q8/led");
        }
    }
    mqtt.loop();
}

void MqttServer::publishMessage(const char* topic, const char* message) {
    mqtt.publish(topic, message);
}

void MqttServer::mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Convert payload to string
    String message;
    message.reserve(length + 1);
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.printf("[Server] Received MQTT message on topic '%s': '%s'\r\n", topic, message.c_str());
    
    String topicStr = String(topic);
    
    if (topicStr == "x7k9m2q8/hello") {
        instance->handleHelloMessage(message);
    } else if (topicStr == "x7k9m2q8/led") {
        instance->handleLedMessage(message);
    }
}

void MqttServer::handleHelloMessage(const String& message) {
    // Respond to any message on /hello topic with "world"
    Serial.printf("[Server] hello_handler: received '%s'\r\n", message.c_str());
    publishMessage("x7k9m2q8/hello/response", "world");
}

void MqttServer::handleLedMessage(const String& message) {
    Serial.printf("[Server] led_handler: received '%s'\r\n", message.c_str());
    
    String msg = message;
    msg.trim();
    msg.toLowerCase();
    
    if (msg == "get") {
        bool on = digitalRead(LED_BUILTIN) == HIGH;
        publishMessage("x7k9m2q8/led/response", on ? "on" : "off");
    } else if (msg == "on" || msg == "1" || msg == "true") {
        digitalWrite(LED_BUILTIN, HIGH);
        publishMessage("x7k9m2q8/led/response", "ok: on");
    } else if (msg == "off" || msg == "0" || msg == "false") {
        digitalWrite(LED_BUILTIN, LOW);
        publishMessage("x7k9m2q8/led/response", "ok: off");
    } else {
        // Invalid message
        publishMessage("x7k9m2q8/led/response", "error: expected 'on', 'off', or 'get'");
    }
}