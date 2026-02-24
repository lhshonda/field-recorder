#include <Audio.h>
#include "app.h"
#include "console.h"

// Audio Graph
AudioInputAnalog adc1(A0);
AudioRecordQueue queue1;
AudioConnection patchCord1(adc1, queue1);

// Global App Instance
Application g_app;
Console g_console;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    // Waiting for Serial to initialize
    delay(1000);

    // Allocating Audio Blocks
    AudioMemory(60);

    g_app.setup();
    g_console.init(&g_app);
}

void loop() {
    g_console.update();
    g_app.update();
    
    // Small delay for stability
    delay(10);
}