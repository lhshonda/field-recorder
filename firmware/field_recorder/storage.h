#pragma once
#include <Arduino.h>

// Arduio SD card library
#include <SD.h>
#include "config.h"

// Wav File Writing, look into this
struct WAVHeader {
    char riffId[4];
    uint32_t riffSize;
    char riffFormat[4];

    char fmtId[4];
    uint32_t fmtSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;

    char dataId[4];
    uint32_t dataSize;
};

class Storage {
public:
    Storage();

    // Boolean return values for error handling in future development
    bool init();
    bool isReady() const;

    bool startRecording(const char* filename = nullptr);

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
    bool makeNextFilename(char* outPath, size_t outPathSize);
};

static_assert(sizeof(WAVHeader) == 44);