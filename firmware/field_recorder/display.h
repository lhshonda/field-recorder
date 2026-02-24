#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

// Forward declaration since Display doesn't believe Application exists
class Application;

// OLED Status Display
class Display {
public:
    Display();

    bool init();
    void update(Application* app);
    void showError(const char* message);

private:
    Adafruit_SSD1306 oled;
    bool ready;

    void drawState(DeviceState state);
    void drawStorageStatus(bool storageReady);
};