#pragma once
#include <Arduino.h>
#include "app.h"

// Serial console for debugging and user interaction
class Console {
public:
    Console();

    void init(Application* pApp);
    void update();

private:
    Application* pApp;
    String inputBuffer;

    //  Unmodifiable reference
    void processCommand(const String& command);
    void printHelp();
    void printState();
};