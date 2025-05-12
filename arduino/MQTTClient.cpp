#include "MQTTClient.h"

// Initialize static member
MQTTClient* MQTTClient::_instance = nullptr;

MQTTClient::MQTTClient(
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
) : _broker(broker), 
    _port(port), 
    _clientId(clientId),
    _rootCA(rootCA),
    _certificate(certificate),
    _privateKey(privateKey),
    _updateTopic(updateTopic),
    _updateDeltaTopic(updateDeltaTopic),
    _client(_wifiClient),
    _sensorManager(sensorManager),
    _actuatorManager(actuatorManager) {
    
    _instance = this; // Set instance for static callback
}

void MQTTClient::initialize() {
    _wifiClient.setCACert(_rootCA);
    _wifiClient.setCertificate(_certificate);
    _wifiClient.setPrivateKey(_privateKey);
    
    _client.setServer(_broker, _port);
    _client.setCallback(wrapperCallback);
}

void MQTTClient::connect() {
    while (!_client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (_client.connect(_clientId)) {
            Serial.println("connected");
            _client.subscribe(_updateDeltaTopic);
            Serial.println("Subscribed to " + String(_updateDeltaTopic));
            
            // Report device state immediately after connection
            delay(100); // Short delay to ensure subscription is complete
            reportDeviceState();
        } else {
            Serial.print("failed, rc=");
            Serial.print(_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

bool MQTTClient::isConnected() const {
    return const_cast<PubSubClient&>(_client).connected();

}

void MQTTClient::loop() {
    _client.loop();
}

void MQTTClient::reportDeviceState() {
    _outputDoc.clear();
    JsonObject reportedState = _outputDoc["state"].createNestedObject("reported");
    reportedState["bomba"] = _actuatorManager.getPumpState();
    reportedState["gas_ppm"] = _sensorManager.getGasLevel();

    serializeJson(_outputDoc, _outputBuffer);
    _client.publish(_updateTopic, _outputBuffer);
    Serial.println("Reported state: " + String(_outputBuffer));
}

// Static wrapper for the callback function
void MQTTClient::wrapperCallback(char* topic, byte* payload, unsigned int length) {
    if (_instance) {
        _instance->handleCallback(topic, payload, length);
    }
}

void MQTTClient::handleCallback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message from topic " + String(topic) + ": " + message);

    DeserializationError err = deserializeJson(_inputDoc, payload);
    if (!err) {
        if (String(topic) == _updateDeltaTopic) {
            JsonVariant bomba = _inputDoc["state"]["bomba"];
            if (!bomba.isNull()) {
                bool pumpState = bomba.as<bool>();
                _actuatorManager.setPumpState(pumpState);
                reportDeviceState();
            }
        }
    } else {
        Serial.print("deserializeJson() failed: ");
        Serial.println(err.c_str());
    }
}
