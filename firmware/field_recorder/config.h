#pragma once
#include <Arduino.h>

// Input Pins
#define RECORD_BUTTON_PIN 2
#define BUTTON_DEBOUNCE_MS 60

// Serial Port Communicatio Speed (Default: 115200 baud)
#define SERIAL_BAUD_RATE 115200

// Field Recorder States
enum class DeviceState : uint8_t {
  IDLE,
  ARMED,
  RECORDING,
  PLAYBACK
};

// Audio settings
#define AUDIO_SAMPLE_RATE 16000
#define AUDIO_BIT_DEPTH 16
#define AUDIO_CHANNELS 1
#define AUDIO_BLOCK_SIZE 256

// Storage Settings
#define SD_CHIP_SELECT BUILTIN_SDCARD
#define SAMPLES_DIRECTORY "/samples"
#define MAX_FILENAME_LENGTH 32
#define MAX_SAMPLES 100











