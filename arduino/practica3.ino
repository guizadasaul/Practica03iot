#include <Arduino.h>
#include "config.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "ActuatorManager.h"
#include "MQTTClient.h"

// Create class instances
WiFiManager wifiManager(WIFI_SSID, WIFI_PASS);
SensorManager sensorManager(MQ2_SENSOR_PIN, SENSOR_READ_INTERVAL);
ActuatorManager actuatorManager(PUMP_PIN);
MQTTClient mqttClient(
    MQTT_BROKER, 
    MQTT_PORT, 
    CLIENT_ID,
    AMAZON_ROOT_CA1,
    CERTIFICATE,
    PRIVATE_KEY,
    UPDATE_TOPIC,
    UPDATE_DELTA_TOPIC,
    sensorManager,
    actuatorManager
);

void setup() {
    Serial.begin(115200);
    Serial.println("Starting system initialization...");
    
    // Initialize components
    actuatorManager.initialize();
    sensorManager.initialize();
    
    // Connect to WiFi
    wifiManager.connect();
    
    // Initialize MQTT client
    mqttClient.initialize();
    
    Serial.println("System initialization complete!");
}

void loop() {
    // Check MQTT connection
    if (!mqttClient.isConnected()) {
        mqttClient.connect();
    }
    mqttClient.loop();
    
    // Check if it's time to read sensor
    if (sensorManager.isTimeToRead()) {
        sensorManager.update();
        mqttClient.reportDeviceState();
    }
}
