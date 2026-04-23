#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "Hotspot";
const char* password = "sheilakijawani";
String apiKey = "W0HWYOMZ4V9GWGXK";
const char* server = "http://api.thingspeak.com/update";

#define VIBRATION_PIN 5
#define CURRENT_PIN 34
#define TEMP_PIN 4

#define CURRENT_THRESHOLD 0.5
#define TEMP_THRESHOLD 45.0

OneWire oneWire(TEMP_PIN);
DallasTemperature tempSensor(&oneWire);

void setup() {
    Serial.begin(115200);
    pinMode(VIBRATION_PIN, INPUT_PULLUP);
    tempSensor.begin();
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void loop() {
    // Read all sensors
    int vibration = digitalRead(VIBRATION_PIN);

    int sensorValue = analogRead(CURRENT_PIN);
    float voltage = sensorValue * (3.3 / 4095.0);
    float current = (voltage - 1.65) / 0.185;

    tempSensor.requestTemperatures();
    float temperature = tempSensor.getTempCByIndex(0);

    // Print readings
    Serial.print("Vibration: "); Serial.print(vibration);
    Serial.print(" | Current: "); Serial.print(current); Serial.print("A");
    Serial.print(" | Temp: "); Serial.print(temperature); Serial.println("C");

    // Check thresholds and alert
    if (vibration == 1) {
        Serial.println("⚠ ALERT: Abnormal vibration detected!");
    }
    if (current > CURRENT_THRESHOLD) {
        Serial.println("⚠ ALERT: Current too high - possible blockage!");
    }
    if (temperature > TEMP_THRESHOLD) {
        Serial.println("⚠ ALERT: Overheating detected!");
    }

    // Combined fault detection
    if (vibration == 1 && current > CURRENT_THRESHOLD) {
        Serial.println("🔴 CRITICAL: Vibration + High Current = Mechanical Fault!");
    }
    if (current > CURRENT_THRESHOLD && temperature > TEMP_THRESHOLD) {
        Serial.println("🔴 CRITICAL: High Current + Overheating = Motor Failure Risk!");
    }

    // Send to ThingSpeak
    sendToThingSpeak(vibration, current, temperature);
    delay(15000);
}

void sendToThingSpeak(int vibration, float current, float temperature) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = server;
        url += "?api_key=" + apiKey;
        url += "&field1=" + String(vibration);
        url += "&field2=" + String(current);
        url += "&field3=" + String(temperature);
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.println("Data sent successfully!");
        } else {
            Serial.println("Error sending data");
        }
        http.end();
    } else {
        Serial.println("WiFi disconnected!");
    }
}