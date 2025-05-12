#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

class SensorManager {
private:
    int _sensorPin;
    int _gasLevel;
    unsigned long _lastReadTime;
    unsigned long _readInterval;

public:
    // Constructor
    SensorManager(int sensorPin, unsigned long readInterval);
    
    // Methods
    void initialize();
    void update();
    int getGasLevel() const;
    bool isTimeToRead() const;
};

#endif // SENSOR_MANAGER_H
