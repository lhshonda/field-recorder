#include "storage.h"

Storage::Storage() : ready(false), bytesWritten(0) {
}


// Writing all the mock storage methods with simple print statements for now.
bool Storage::init() {
    // TODO: Implement SD card initialization logic here
    ready = true;
    Serial.println("Storage (mock) init successful.");
    return ready;
}

bool Storage::isReady() const {
    return ready;
}

// Low-Level C libraries expect 'char*' pointers rather than the Arduino 'String' objects.
bool Storage::startRecording(const char* filename) {
    if (!ready) return false;
    Serial.print("Starting recording (mock): ");
    Serial.println(filename);
    bytesWritten = 0;
    return true;
}

bool Storage::writeAudioData(const int16_t* buffer, size_t numSamples) {
    if (!ready) return false;

    // Writing mock data for now
    bytesWritten += numSamples * sizeof(int16_t);
    return true;
}

bool Storage::stopRecording() {
    if (!ready) return false;
    Serial.print("Stopped recording. Total bytes written: ");
    Serial.println(bytesWritten);
    return true;
}

bool Storage::startPlayback(const char* filename) {
    if (!ready) return false;
    Serial.print("Starting playback (mock): ");
    Serial.println(filename);
    return true;
}

bool Storage::readAudioData(int16_t* buffer, size_t numSamples) {
    if (!ready) return false;

    // Reading mock data for now (filling buffer with zeros)
    // memset() is a fast C function used to fill memory with a specific value.
    // I could use a for loop but this is faster.
    memset(buffer, 0, numSamples * sizeof(int16_t));
    return true;
}

bool Storage::stopPlayback() {
    if (!ready) return false;
    Serial.println("Stopped playback.");
    return true;
}

void Storage::listSamples() const {
    Serial.println("Listing samples...");
    Serial.println("  sample1.wav");
    Serial.println("  sample2.wav");
}