#include "storage.h"

Storage::Storage() : ready(false), bytesWritten(0) {
}


// Writing all the mock storage methods with simple print statements for now.
bool Storage::init() {
    // SD Card Logic
    Serial.println("Intializing storage...");
    
    if (!SD.begin(SD_CHIP_SELECT)) {
        Serial.println("Error: SD card initialization failed!");
        ready = false;
        return ready;
    }
    
    Serial.println("OK");
    ready = true;
    
    if (!SD.exists(SAMPLES_DIRECTORY)) {
        Serial.print("Samples directory not found. Creating... ");
        if (SD.mkdir(SAMPLES_DIRECTORY)) {
            Serial.println("Successfully created.");
        } else {
            Serial.println("Failed to create samples directory!");
        }
    }

    return ready;
}

bool Storage::isReady() const {
    return ready;
}

// Low-Level C libraries expect 'char*' pointers rather than the Arduino 'String' objects.
bool Storage::startRecording(const char* filename) {
    if (!ready) return false;
 
    // Building the full path for the new recording.
    // char array for SD library
    char filepath[MAX_FILENAME_LENGTH + 20];

    if (filename && filename[0] != '\0') {
        // Safely formating the string with the directory and filename into the filepath buffer.
        snprintf(filepath, sizeof(filepath), "%s/%s", SAMPLES_DIRECTORY, filename);
    } else {
        if (!makeNextFilename(filepath, sizeof(filepath))) {
            Serial.println("Error: Failed to generate a filename for recording!");
            return false;
        }
    }
    
    // Using unique file names to prevent overwriting existing recordings
    recordFile = SD.open(filepath, FILE_WRITE);
    if (!recordFile) {
        Serial.println("Error: Failed to create file for recording!");
        Serial.println(filepath);
        return false;
    }
    
    // WAVHeader
    WAVHeader header = {

        // RIFF chunk descriptor
        .riffId = {'R', 'I', 'F', 'F'},           // "RIFF"
        .riffSize = 0,                            // File size - 8 bytes  
        .riffFormat = {'W', 'A', 'V', 'E'},       // "WAVE"

        // fmt sub-chunk
        .fmtId = {'f', 'm', 't', ' '},            // "fmt "
        .fmtSize = 16,                            // 16 for Standard PCM
        .audioFormat = 1,                         // 1 for PCM
        .numChannels = 1,                         // Mono  
        .sampleRate = AUDIO_SAMPLE_RATE,
        .byteRate = AUDIO_SAMPLE_RATE * AUDIO_CHANNELS * (AUDIO_BIT_DEPTH / 8),
        .blockAlign = AUDIO_CHANNELS * (AUDIO_BIT_DEPTH / 8),
        .bitsPerSample = AUDIO_BIT_DEPTH,

        .dataId = {'d', 'a', 't', 'a'},           // "data"
        .dataSize = 0                             // Number of bytes of PCM data
    };

    // Writing header to file
    recordFile.write((uint8_t*)&header, sizeof(WAVHeader));
    bytesWritten = 0;

    Serial.print("Started recording to: ");
    Serial.println(filepath);
    return true;
}

bool Storage::writeAudioData(const int16_t* buffer, size_t numSamples) {
    if (!ready || !recordFile) return false;

    size_t bytesToWrite = numSamples * sizeof(int16_t);
    size_t written = recordFile.write((const uint8_t*)buffer, bytesToWrite);

    if (written != bytesToWrite) {
        Serial.println("Error: Failed to write all audio data to file!");
        return false;
    }

    bytesWritten += written;
    return true;
}

bool Storage::stopRecording() {
    if (!ready || !recordFile) return false;

    // Updating WAV header with the right sizes
    // Header plus the data
    uint32_t fileSize = 36 + bytesWritten;
    uint32_t dataSize = bytesWritten;

    // WAV header offset 4 = RIFF chunk size field (file size minus 8 bytes for RIFF header)
    recordFile.seek(4);
    recordFile.write((uint8_t*)&fileSize, sizeof(fileSize));

    // WAV header offset 40 = data chunk size field
    recordFile.seek(40);
    recordFile.write((uint8_t*)&dataSize, sizeof(dataSize));

    recordFile.close();

    Serial.println("Stopped recording.");
    Serial.print("Total bytes written: ");
    Serial.println(fileSize + 8);
    Serial.print(" bytes.");

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

    // Reading mock data for now (memset() better than for loop)
    memset(buffer, 0, numSamples * sizeof(int16_t));
    return true;
}

bool Storage::stopPlayback() {
    if (!ready) return false;
    Serial.println("Stopped playback.");
    return true;
}

bool Storage::makeNextFilename(char* outPath, size_t outPathSize) {
    if (!outPath || outPathSize == 0) return false;

    for (uint16_t i = 1; i <= MAX_SAMPLES; ++i) {
        // e.g. /samples/REC0001.WAV
        snprintf(outPath, outPathSize, "%s/REC%04u.WAV", SAMPLES_DIRECTORY, i);

        if (!SD.exists(outPath)) {
            return true;
        }
    }

    Serial.println("Error: No available filenames for recording!");
    return false;
}

// Placeholder method for listing samples
void Storage::listSamples() const {
    Serial.println("Listing samples...");
}