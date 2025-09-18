#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttClient {
private:
    WiFiClient wifiClient;
    PubSubClient mqtt;
    
    // Scheduling variables
    unsigned long helloAtMs;
    bool helloSent;
    unsigned long nextMqttLedToggleMs;
    bool nextLedStateOn;
    
    static const uint32_t ledTogglePeriodMs = 2000;
    
    // Message callback for responses
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    
    // Static instance pointer for static callback
    static MqttClient* instance;

public:
    MqttClient();
    void begin();
    void loop();
};

#endif // MQTT_CLIENT_H