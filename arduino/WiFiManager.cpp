#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password) 
    : _ssid(ssid), _password(password) {
}

void WiFiManager::connect() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(_ssid);
    
    WiFi.disconnect(true); // Reset WiFi configuration
    delay(1000);           // Wait to ensure WiFi reset
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Connected to WiFi. IP address: ");
    Serial.println(WiFi.localIP());
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WiFiManager::getLocalIP() const {
    return WiFi.localIP();
}
