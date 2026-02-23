#include "app.h"
#include "console.h"

// Global App Instance
Application g_app;
Console g_console;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    // Waiting for Serial to initialize
    delay(1000);

    g_app.setup();
    g_console.init(&g_app);
}

void loop() {
    g_app.update();
    g_console.update();
    
    // Small delay for stability
    delay(10);
}