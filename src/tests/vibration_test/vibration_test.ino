#define VIBRATION_PIN 5

void setup() {
    Serial.begin(115200);
    pinMode(VIBRATION_PIN, INPUT_PULLUP);
    Serial.println("Vibration Sensor Test");
    Serial.println("Tap the sensor to test!");
}

void loop() {
    int vibration = digitalRead(VIBRATION_PIN);
    if (vibration == 1) {
        Serial.println("VIBRATION DETECTED!");
    } else {
        Serial.println("No vibration");
    }
    delay(100);
}