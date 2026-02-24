#include "app.h"

// Constructor initializes the device state to IDLE
// Initializer list is more efficient here than assigning in the constructor body
Application::Application() : currentState(DeviceState::IDLE) {
}

// Placeholder for future setup code, such as initializing SD card, audio hardware, etc.
void Application::setup() {
    Serial.println("Intializing Field Recorder...");

    if (!storage.init()) {
        Serial.println("Error: Storage initialization failed!");
    }
}

// Call specific handler based on recorder state.
void Application::update() {
    switch (currentState) {
        case DeviceState::IDLE:
            handleIdleUpdate();
            break;
        case DeviceState::ARMED:
            handleArmedUpdate();
            break;
        case DeviceState::RECORDING:
            handleRecordingUpdate();
            break;
        case DeviceState::PLAYBACK:
            handlePlaybackUpdate();
            break;
    }    
}

// State transition methods
void Application::arm() {
    if (currentState == DeviceState::IDLE) {
        currentState = DeviceState::ARMED;
        Serial.println("Device armed. Ready to record.");
    }
}
void Application::disarm() {
    if (currentState == DeviceState::ARMED) {
        currentState = DeviceState::IDLE;
        Serial.println("Device disarmed. Back to idle.");
    }
}
void Application::startRecording() {
    if (currentState == DeviceState::ARMED) {
        currentState = DeviceState::RECORDING;
        storage.startRecording("sample.wav"); // TODO: Generate unique filename
        Serial.println("Recording started.");
    }
}
void Application::stopRecording() {
    if (currentState == DeviceState::RECORDING) {
        storage.stopRecording();
        currentState = DeviceState::ARMED;
        Serial.println("Recording stopped. Device is still armed.");
    }
}
void Application::startPlayback() {
    if (currentState == DeviceState::IDLE || currentState == DeviceState::ARMED) {
        currentState = DeviceState::PLAYBACK;
        Serial.println("Playback started.");
    }
}
void Application::stopPlayback() {
    if (currentState == DeviceState::PLAYBACK) {
        currentState = DeviceState::IDLE;
        Serial.println("Playback stopped. Back to idle.");
    }
}

// Returning state of device, using const to prevent accidental state changes
DeviceState Application::getState() const {
    return currentState;
}

// Placeholder methods for handling updates in each state
void Application::handleIdleUpdate() {
    // TODO
}
void Application::handleArmedUpdate() {
    // TODO
}
void Application::handleRecordingUpdate() {
    // TODO
}
void Application::handlePlaybackUpdate() {
    // TODO
}
