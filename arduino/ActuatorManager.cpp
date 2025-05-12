#include "ActuatorManager.h"

ActuatorManager::ActuatorManager(int pumpPin)
    : _pumpPin(pumpPin), _pumpState(false) {
}

void ActuatorManager::initialize() {
    pinMode(_pumpPin, OUTPUT);
    setPumpState(false); // Ensure pump is off initially
}

void ActuatorManager::setPumpState(bool state) {
    _pumpState = state;
    digitalWrite(_pumpPin, _pumpState ? HIGH : LOW);
    Serial.println(_pumpState ? "Pump turned ON" : "Pump turned OFF");
}

bool ActuatorManager::getPumpState() const {
    return _pumpState;
}
