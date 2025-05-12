#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <Arduino.h>

class WiFiManager {
private:
    const char* _ssid;
    const char* _password;

public:
    // Constructor
    WiFiManager(const char* ssid, const char* password);
    
    // Methods
    void connect();
    bool isConnected() const;
    IPAddress getLocalIP() const;
};

#endif // WIFI_MANAGER_H
