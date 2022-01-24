#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>

// create a MQTT_config.h file with SSID, PASSWORD, MQTT_SERVER and MQTT_port 
#include "MQTT_config.h"

#define DHTPIN 34     
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    Serial.println(F("DHTxx test!"));

    dht.begin();
}

void loop() {
    delay(2000);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

}