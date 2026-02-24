#include "app.h"

// Constructor initializes the device state to IDLE
// Initializer list is more efficient here than assigning in the constructor body
Application::Application() : currentState(DeviceState::IDLE), 
    lastDebounceMs(0) {
}

void Application::setup() {
    Serial.println("Initializing Field Recorder...");

    // Activating internal resistors with INPUT_PULLUP for the record button pin
    pinMode(RECORD_BUTTON_PIN, INPUT_PULLUP);
    
    bool initial = digitalRead(RECORD_BUTTON_PIN);
    lastRecordButtonState = initial;
    stableRecordButtonState = initial;
    lastDebounceMs = millis();

    // Error Handling
    if (!storage.init()) {
        Serial.println("Error: Storage initialization failed!");
    }

    if (!display.init()) {
        Serial.println("Error: Display initialization failed!");
    }
}

// Call specific handler based on recorder state.
void Application::update() {
    debugButtonRaw();

    switch (currentState) {
        case DeviceState::IDLE: handleIdleUpdate(); break;
        case DeviceState::ARMED: handleArmedUpdate(); break;
        case DeviceState::RECORDING: handleRecordingUpdate(); break;
        case DeviceState::PLAYBACK: handlePlaybackUpdate(); break;
    }
    
    // Update the display with current state and storage status
    handleRecordButton();
    display.update(this);
}

// New method to handle record button state and debounce logic
void Application::handleRecordButton() {
    bool reading = digitalRead(RECORD_BUTTON_PIN);

    // lastRecordButtonState tracks the last raw reading from the button pin
    if (reading != lastRecordButtonState) {
        // Reset the debounce timer mark
        lastDebounceMs = millis();
        lastRecordButtonState = reading;
    }

    // stableRecordButtonState is the debounced state
    if ((millis() - lastDebounceMs) > BUTTON_DEBOUNCE_MS) {
        if (stableRecordButtonState != reading) {
            stableRecordButtonState = reading;

            // Debounce Debugging
            Serial.print("BTN debounced: ");
            Serial.println(stableRecordButtonState == LOW ? "PRESSED" : "RELASED");

            if (stableRecordButtonState == LOW) {
                if (currentState == DeviceState::IDLE) arm();
                else if (currentState == DeviceState::ARMED) startRecording();
                else if (currentState == DeviceState::RECORDING) stopRecording();
            }
        }
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
        if (storage.startRecording()) {
            currentState = DeviceState::RECORDING;
            Serial.println("Recording started.");
        } else {
            Serial.println("Error: Failed to start recording!");
        }
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

// Temporary mock implementation for recording update, simulating audio data writing, this currently writes silence.
void Application::handleRecordingUpdate() {
    static uint32_t lastWriteMs = 0;

    // 20ms per audio chunk to mimic real-time recording
    if (millis() - lastWriteMs < 20) return;
    lastWriteMs = millis();

    // 16k samples per second * 0.02 seconds = 320 mono samples per chunk
    int16_t buffer[320] = {0};
    if (!storage.writeAudioData(buffer, 320)) {
        Serial.println("Write failed, stopping recording.");
        stopRecording();
    }
}

// Debug Method (Momentary switch is being finnicky)
void Application::debugButtonRaw() {
    static uint32_t lastPrintMs = 0;
    if (millis() - lastPrintMs < 50) return; // 20 Hz print
    lastPrintMs = millis();

    bool raw = digitalRead(RECORD_BUTTON_PIN);
    Serial.print("BTN raw=");
    Serial.println(raw ? "HIGH (released)" : "LOW (pressed)");
}


void Application::handleIdleUpdate() {
    // TODO
}

void Application::handleArmedUpdate() {
    // TODO
}

void Application::handlePlaybackUpdate() {
    // TODO
}
