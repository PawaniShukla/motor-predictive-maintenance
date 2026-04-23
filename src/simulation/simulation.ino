#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Hotspot";
const char* password = "sheilakijawani";
String apiKey = "W0HWYOMZ4V9GWGXK";
const char* server = "http://api.thingspeak.com/update";

#define CURRENT_THRESHOLD 0.5
#define TEMP_THRESHOLD 45.0

// Simulation variables
float simulatedTemp = 28.0;
float simulatedCurrent = 0.15;
int simulatedVibration = 0;
int cycleCount = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.println("Starting simulation...");
}

void loop() {
    cycleCount++;

    // PHASE 1 (cycles 1-5): Normal operation
    if (cycleCount <= 5) {
        simulatedVibration = 0;
        simulatedCurrent = 0.15 + random(-10, 10) / 100.0;  // 0.05 - 0.25A
        simulatedTemp = 28.0 + random(0, 50) / 10.0;         // 28 - 33°C
        Serial.println("--- Phase: NORMAL OPERATION ---");
    }

    // PHASE 2 (cycles 6-10): Early fault - vibration starts
    else if (cycleCount <= 10) {
        simulatedVibration = random(0, 3) == 0 ? 1 : 0;      // occasional vibration
        simulatedCurrent = 0.3 + random(0, 20) / 100.0;      // 0.3 - 0.5A
        simulatedTemp = 35.0 + random(0, 50) / 10.0;         // 35 - 40°C
        Serial.println("--- Phase: EARLY FAULT ---");
    }

    // PHASE 3 (cycles 11-15): Developing fault - all sensors affected
    else if (cycleCount <= 15) {
        simulatedVibration = random(0, 2) == 0 ? 1 : 0;      // frequent vibration
        simulatedCurrent = 0.5 + random(0, 30) / 100.0;      // 0.5 - 0.8A
        simulatedTemp = 42.0 + random(0, 80) / 10.0;         // 42 - 50°C
        Serial.println("--- Phase: DEVELOPING FAULT ---");
    }

    // PHASE 4 (cycles 16+): Critical fault
    else {
        simulatedVibration = 1;                                // constant vibration
        simulatedCurrent = 0.8 + random(0, 40) / 100.0;      // 0.8 - 1.2A
        simulatedTemp = 52.0 + random(0, 100) / 10.0;        // 52 - 62°C
        Serial.println("--- Phase: CRITICAL FAULT ---");
        if (cycleCount > 20) cycleCount = 1;                  // reset after full cycle
    }

    // Print readings
    Serial.print("Vibration: "); Serial.print(simulatedVibration);
    Serial.print(" | Current: "); Serial.print(simulatedCurrent); Serial.print("A");
    Serial.print(" | Temp: "); Serial.print(simulatedTemp); Serial.println("C");

    // Check thresholds and alert
    if (simulatedVibration == 1) {
        Serial.println("⚠ ALERT: Abnormal vibration detected!");
    }
    if (simulatedCurrent > CURRENT_THRESHOLD) {
        Serial.println("⚠ ALERT: Current too high!");
    }
    if (simulatedTemp > TEMP_THRESHOLD) {
        Serial.println("⚠ ALERT: Overheating detected!");
    }

    // Combined fault detection
    if (simulatedVibration == 1 && simulatedCurrent > CURRENT_THRESHOLD) {
        Serial.println("🔴 CRITICAL: Vibration + High Current = Mechanical Fault!");
    }
    if (simulatedCurrent > CURRENT_THRESHOLD && simulatedTemp > TEMP_THRESHOLD) {
        Serial.println("🔴 CRITICAL: High Current + Overheating = Motor Failure Risk!");
    }

    Serial.print("Cycle: "); Serial.println(cycleCount);
    Serial.println("---------------------------");

    // Send to ThingSpeak
    sendToThingSpeak(simulatedVibration, simulatedCurrent, simulatedTemp);
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
            Serial.println("Data sent to ThingSpeak!");
        } else {
            Serial.println("Error sending data");
        }
        http.end();
    } else {
        Serial.println("WiFi disconnected!");
    }
}