#include "app.h"

// Global App Instance
Application g_app;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // giving the serial monitor some time to initialize
    g_app.setup();
}

void loop() {
    g_app.update();
    delay(10); // small delay for stability. change later for audio processing timing
}