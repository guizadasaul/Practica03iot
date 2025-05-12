#include "SensorManager.h"

SensorManager::SensorManager(int sensorPin, unsigned long readInterval)
    : _sensorPin(sensorPin), _gasLevel(0), _lastReadTime(0), _readInterval(readInterval) {
}

void SensorManager::initialize() {
    pinMode(_sensorPin, INPUT);
    // Perform initial reading
    update();
}

void SensorManager::update() {
    int sensorValue = analogRead(_sensorPin);
    _gasLevel = map(sensorValue, 0, 4095, 0, 1000);
    _lastReadTime = millis();
    
    Serial.print("Gas sensor reading: ");
    Serial.print(sensorValue);
    Serial.print(" (Raw), ");
    Serial.print(_gasLevel);
    Serial.println(" PPM");
}

int SensorManager::getGasLevel() const {
    return _gasLevel;
}

bool SensorManager::isTimeToRead() const {
    return (millis() - _lastReadTime) > _readInterval;
}
