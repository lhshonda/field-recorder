#include "display.h"
#include "app.h"

// OLED dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C // Might be 0x3D

Display::Display() : oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), ready(false) {
}

bool Display::init() {
    if (!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("ERROR: OLED Init failed!");
        return false;
    }
    
    ready = true;

    // Initial display setup
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println("Field Recorder");
    oled.println("Initializing...");
    oled.display();

    // Simulate initialization time for "better" user experience, lol
    delay(1500);

    Serial.println("OLED display init successful.");
    return true;
}

void Display::update(Application* pApp) {
    if (!ready) return;

    // Get display ready for new content
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);

    // Draw current state
    DeviceState state = pApp->getState();
    drawState(state);

    oled.setTextSize(1);
    oled.setCursor(0, 40);

    // TODO: PLACEHOLDER foor storage status
    drawStorageStatus(true);

    // Update the display with new content
    oled.display();
}

void Display::drawState(DeviceState state) {
    switch (state) {
        case DeviceState::IDLE:
            oled.println("IDLE");
            break;
        case DeviceState::ARMED:
            oled.println("ARMED");
            break;
        case DeviceState::RECORDING:
            oled.println("REC");
            break;
        case DeviceState::PLAYBACK:
            oled.println("PLAY");
            break;
    }
}

void Display::drawStorageStatus(bool storageReady) {
    oled.print("Storage: ");
    if (storageReady) {
        oled.println("OK");
    } else {
        oled.println("ERR");
    }
}

void Display::showError(const char* message) {
    if (!ready) return;

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println("ERROR:");
    oled.println(message);
    oled.display();
}