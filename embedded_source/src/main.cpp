/*
 * Add debug box on node-red 
 */


#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>

#include "MQTT_config.h"

#if !defined(SSID) || !defined(PASSWORD) || !defined(MQTT_SERVER) || !defined(MQTT_PORT)
    #define SSID "network"
    #define PASSWORD "password"
    #define MQTT_SERVER "127.000.000.001"
    #define MQTT_PORT 1883
#endif

#define DHTPIN 33
#define DHTTYPE DHT22 

#define LNAVOLTAGEPIN 32
#define LNACONTROLPIN 16

#define DELAYMS 2000

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float h, t, hic, LNA_voltage;

void setup_wifi(void);
void callback(char*, byte*, unsigned int);
void reconnect(void); 
void read_sensors(void);
void MQTT_publish(void);

void setup() {
    pinMode(LNAVOLTAGEPIN, INPUT_PULLDOWN);
    pinMode(LNACONTROLPIN, OUTPUT);

    Serial.begin(115200);
    Serial.println(F("\n\nBINGO TELESCOPE embedded system!"));

    dht.begin();

    setup_wifi();
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected())
        reconnect();
    client.loop();

    long now = millis();
    if (now - lastMsg > DELAYMS) {
        lastMsg = now;

        read_sensors();

        MQTT_publish();
    }

}

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;
    
    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // If a message is received on the topic esp32/LNA_control, check if the message is either "on" or "off". 
    // Changes the output state according to the message
    if (String(topic) == "esp32/LNA_control") {
        Serial.print("Changing LNA_control to ");
        if(messageTemp == "on"){
            Serial.println("on");
            digitalWrite(LNACONTROLPIN, LOW);
        }
        else if(messageTemp == "off"){
            Serial.println("off");
            digitalWrite(LNACONTROLPIN, HIGH);
        }
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP32Client")) {
            Serial.println("connected");
            // Subscribe
            client.subscribe("esp32/LNA_control");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void read_sensors(void) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    t = dht.readTemperature();
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        t = -1;
        h = -1;
        hic = -1;
    }
    else
        // Compute heat index in Celsius (isFahreheit = false)
        hic = dht.computeHeatIndex(t, h, false);

    // read and calculate voltage level
    Serial.println(analogRead(LNAVOLTAGEPIN));
    LNA_voltage = 3.3 * (analogRead(LNAVOLTAGEPIN) / 4095.0);
}

void MQTT_publish(void) {
    // Convert the temperature to a char array
    char tempString[8];
    dtostrf(t, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.print(tempString);
    Serial.print("°C\t");
    client.publish("esp32/temperature", tempString);

    // Convert the humidity to a char array
    char humiString[8];
    dtostrf(h, 1, 2, humiString);
    Serial.print("Humidity: ");
    Serial.print(humiString);
    Serial.print("%\t");
    client.publish("esp32/humidity", humiString);

    // Convert the heat_index to a char array
    char indexString[8];
    dtostrf(hic, 1, 2, indexString);
    Serial.print("Heat index: ");
    Serial.print(indexString);
    Serial.print("°C\t");
    client.publish("esp32/heat_index", indexString);

    // Convert the LNA_voltage to a char array
    char voltageString[8];
    dtostrf(LNA_voltage, 1, 2, voltageString);
    Serial.print("LNA voltage: ");
    Serial.print(voltageString);
    Serial.print("V\t\n");
    client.publish("esp32/LNA_voltage", voltageString);
}