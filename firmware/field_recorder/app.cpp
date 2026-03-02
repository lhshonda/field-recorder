#include <Audio.h>
#include "app.h"
extern AudioRecordQueue queueL;
extern AudioRecordQueue queueR;

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
    // debugButtonRaw(); // Debugging method for raw button state, currently commented out to prevent flooding serial

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
            // Serial.print("BTN debounced: ");
            // Serial.println(stableRecordButtonState == LOW ? "PRESSED" : "RELASED");

            if (stableRecordButtonState == LOW) {
                if (currentState == DeviceState::IDLE) arm();
                else if (currentState == DeviceState::ARMED) startRecording();
                else if (currentState == DeviceState::RECORDING) stopRecording();
            }
        }
    }
}

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
            queueL.begin();
            queueR.begin();
            currentState = DeviceState::RECORDING;
            Serial.println("Recording started.");
        } else {
            Serial.println("Error: Failed to start recording!");
        }
    }
}

void Application::stopRecording() {
    if (currentState == DeviceState::RECORDING) {
        queueL.end();
        queueR.end();
        if (storage.stopRecording()) {
            currentState = DeviceState::ARMED;
            Serial.println("Recording stopped. Device is still armed.");
        } else {
            currentState = DeviceState::ARMED; // Fallback to safe state
            Serial.println("Error: Failed to stop recording properly!");
        }
    }
}

// Returning state of device, using const to prevent accidental state changes
DeviceState Application::getState() const {
    return currentState;
}

// Real Mic Queue (TeensyAudioLibrary)
void Application::handleRecordingUpdate() {
    // Processes queued audio blocks
    while (queueL.available() > 0 && queueR.available() > 0) {
        // Places pointer on oldest block of audio data in the queue
        int16_t* left = queueL.readBuffer();
        int16_t* right = queueR.readBuffer();

        int16_t interleaved[256]; // 256 samples total, 128 p/channel
        for (int i = 0; i < 128; i++) {
            interleaved[i * 2] = left[i];
            interleaved[i * 2 + 1] = right[i];
        }

        // Append audio block to WAV file as PCM, includes failure handling
        if (!storage.writeAudioData(interleaved, 256)) {
            queueL.freeBuffer();
            queueR.freeBuffer();
            Serial.println("Error: Write failed, aborting recording.");
            stopRecording();
            return;
        }

        // Returns block memory to queue for reuse
        queueL.freeBuffer();
        queueR.freeBuffer();
    }
}

// Debug Method (Momentary switch is being finnicky)
// void Application::debugButtonRaw() {
//     static uint32_t lastPrintMs = 0;
//     if (millis() - lastPrintMs < 50) return; // 20 Hz print
//     lastPrintMs = millis();

//     bool raw = digitalRead(RECORD_BUTTON_PIN);
//     Serial.print("BTN raw=");
//     Serial.println(raw ? "HIGH (released)" : "LOW (pressed)");
// }

// Storage display method
bool Application::isStorageReady() const {
    return storage.isReady();
}

// TODO: IMPLEMENT THESE METHODS IF APPLICABLE IN FUTURE DEVELOPMENT

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

void Application::handleIdleUpdate() {
    // TODO
}

void Application::handleArmedUpdate() {
    // TODO
}

void Application::handlePlaybackUpdate() {
    // TODO
}
