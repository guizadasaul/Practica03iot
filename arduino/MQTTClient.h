#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "SensorManager.h"
#include "ActuatorManager.h"

class MQTTClient {
private:
    const char* _broker;
    int _port;
    const char* _clientId;
    const char* _rootCA;
    const char* _certificate;
    const char* _privateKey;
    const char* _updateTopic;
    const char* _updateDeltaTopic;
    
    WiFiClientSecure _wifiClient;
    PubSubClient _client;
    
    SensorManager& _sensorManager;
    ActuatorManager& _actuatorManager;
    
    StaticJsonDocument<JSON_OBJECT_SIZE(64)> _inputDoc;
    StaticJsonDocument<JSON_OBJECT_SIZE(16)> _outputDoc;
    char _outputBuffer[256];
    
    static MQTTClient* _instance; // For callback
    static void wrapperCallback(char* topic, byte* payload, unsigned int length);
    void handleCallback(char* topic, byte* payload, unsigned int length);

public:
    // Constructor
    MQTTClient(
        const char* broker, 
        int port, 
        const char* clientId,
        const char* rootCA,
        const char* certificate,
        const char* privateKey,
        const char* updateTopic,
        const char* updateDeltaTopic,
        SensorManager& sensorManager,
        ActuatorManager& actuatorManager
    );
    
    // Methods
    void initialize();
    void connect();
    bool isConnected() const;
    void loop();
    void reportDeviceState();
};

#endif // MQTT_CLIENT_H
