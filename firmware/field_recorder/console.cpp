#include "console.h"

Console::Console() {
}

// Two-Phase Construction
void Console::init(Application* app) {
    pApp = app;
    Serial.println("Console initialized. Type 'help' for commands.");
}

void Console::update() {
    // Checking for data on Serial
    if (Serial.available()) {
        char c = Serial.read();

        // Checking for both Newline and Carriage Return to accommodate different terminal settings
        if (c == '\n' || c == '\r') {
            // If anything is in the buffer, process it as a command
            // and then clear the buffer for the next command
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
                inputBuffer = "";
            }
        } else {
            // Append the character to the input buffer
            inputBuffer += c;
        }
    }
}

// Process user commands from the console
void Console::processCommand(const String& command) {
    if (command == "arm") {
        pApp->arm();       
    } else if (command == "disarm") {
        pApp->disarm();
    } else if (command == "record") {
        pApp->startRecording();
    } else if (command == "stop") {
        pApp->stopRecording();
        pApp->stopPlayback();
    } else if (command == "play") {
        pApp->startPlayback();
    } else if (command == "help") {
        printHelp();
    } else if (command == "state") {
        printState();
    } else {
        Serial.println("Unknown command. Type 'help' for a list of commands.");
    }
}

void Console::printHelp() {
    Serial.println("Available commands:");
    Serial.println("  arm     - Arm the device for recording");
    Serial.println("  disarm  - Disarm the device");
    Serial.println("  record  - Start recording (must be armed)");
    Serial.println("  stop    - Stop recording or playback");
    Serial.println("  play    - Start playback (can be from idle or armed)");
    Serial.println("  state   - Print current device state");
    Serial.println("  help    - Show this help message");
}

void Console::printState() {
    DeviceState state = pApp->getState();
    Serial.print("Current State: ");

    switch(state) {
        case DeviceState::IDLE:
            Serial.println("IDLE");
            break;
        case DeviceState::ARMED:
            Serial.println("ARMED");
            break;
        case DeviceState::RECORDING:
            Serial.println("RECORDING");
            break;
        case DeviceState::PLAYBACK:
            Serial.println("PLAYBACK");
            break;
    }
}