#pragma once
#include <Arduino.h>
#include "config.h"
#include "storage.h"
#include "display.h"

class Application {
public:
    Application();

    // Lifecycle methods
    void setup();
    void update();

    // Field Recorder Commands
    void arm();
    void disarm();
    void startRecording();
    void stopRecording();
    void startPlayback();
    void stopPlayback();

    // Query current state
    DeviceState getState() const;

private:
    // Member variable
    DeviceState currentState;
    Storage storage;
    Display display;

    // Button Handling
    bool lastRecordButtonState;
    bool stableRecordButtonState;
    uint32_t lastDebounceMs;

    // Private helper methods
    void handleIdleUpdate();
    void handleArmedUpdate();
    void handleRecordingUpdate();
    void handlePlaybackUpdate();

    // New helper method to check the record button state and handle debouncing
    void handleRecordButton();
};