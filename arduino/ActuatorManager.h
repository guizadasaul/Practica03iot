#ifndef ACTUATOR_MANAGER_H
#define ACTUATOR_MANAGER_H

#include <Arduino.h>

class ActuatorManager {
private:
    int _pumpPin;
    bool _pumpState;

public:
    // Constructor
    ActuatorManager(int pumpPin);
    
    // Methods
    void initialize();
    void setPumpState(bool state);
    bool getPumpState() const;
};

#endif // ACTUATOR_MANAGER_H
