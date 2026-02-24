#pragma once
#include <Arduino.h>

// Arduio SD card library
#include <SD.h>
#include "config.h"

class Storage {
public:
    Storage();

    // Boolean return values for error handling in future development
    bool init();
    bool isReady() const;

    bool startRecording(const char* filename);

    // 16-bit audio samples
    bool writeAudioData(const int16_t* buffer, size_t numSamples);
    bool stopRecording();

    bool startPlayback(const char* filename);
    bool readAudioData(int16_t* buffer, size_t numSamples);
    bool stopPlayback();

    void listSamples() const;

private:
    // Tracking SD initialization
    bool ready;
    File recordFile;
    File playbackFile;
    uint32_t bytesWritten;
};