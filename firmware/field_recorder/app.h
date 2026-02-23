#pragma once
#include <Arduino.h>
#include "config.h"

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

    // Private helper methods
    void handleIdleUpdate();
    void handleArmedUpdate();
    void handleRecordingUpdate();
    void handlePlaybackUpdate();
};