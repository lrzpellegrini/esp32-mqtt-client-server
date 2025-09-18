#ifndef MQTT_SERVER_H
#define MQTT_SERVER_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttServer {
private:
    WiFiClient wifiClient;
    PubSubClient mqtt;
    
    // MQTT callback for incoming messages
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    
    // Message handlers
    void handleHelloMessage(const String& message);
    void handleLedMessage(const String& message);
    
    // Helper function to publish messages
    void publishMessage(const char* topic, const char* message);
    
    // Static instance pointer for static callback
    static MqttServer* instance;

public:
    MqttServer();
    void begin();
    void loop();
};

#endif // MQTT_SERVER_H