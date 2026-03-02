#include <Audio.h>
#include "app.h"
#include "console.h"

// Audio Shield Line-in L/R
AudioInputI2S i2s1;
AudioRecordQueue queueL;
AudioRecordQueue queueR;
AudioConnection patchCord1(i2s1, 0, queueL, 0);
AudioConnection patchCord2(i2s1, 1, queueR, 0);

AudioControlSGTL5000 sgtl5000_1;


// Global App Instance
Application g_app;
Console g_console;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    // Waiting for Serial to initialize
    delay(1000);

    // Allocating Audio Blocks
    AudioMemory(80);

    sgtl5000_1.enable();
    sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
    sgtl5000_1.lineInLevel(5);

    g_app.setup();
    g_console.init(&g_app);
}

void loop() {
    g_console.update();
    g_app.update();
    
    // Small delay for stability
    delay(10);
}