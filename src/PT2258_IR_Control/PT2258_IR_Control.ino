/*
 * PT2258 Audio Volume Controller with IR Remote
 * This project uses an IR remote to control the PT2258 audio chip via I2C.
 * It supports volume increase, decrease, and mute functions.
 *
 * Hardware:
 * - PT2258 Audio Controller
 * - IR Receiver (connected to GPIO14 / D5)
 * - NodeMCU (ESP8266)
 * - I2C (SDA: GPIO4 / D2, SCL: GPIO5 / D1)
 *
 * PT2258 I2C Address Configuration
 * ---------------------------------
 * The PT2258 volume controller IC has configurable I2C addresses based on the 
 * CODE1 and CODE2 pin connections. The corresponding 8-bit and 7-bit addresses 
 * are listed below:
 *
 * | CODE1 | CODE2 | 8-bit Address | 7-bit Address (for Wire library) |
 * |-------|-------|--------------|----------------------------------|
 * |   0   |   0   |    0x80      |    0x40                         |
 * |   1   |   0   |    0x88      |    0x44                         |
 * |   0   |   1   |    0x84      |    0x42                         |
 * |   1   |   1   |    0x8C      |    0x46                         |
 *
 * The Wire library expects a 8-bit address, so if you perform an I2C scan,
 * the detected address will be the right-shifted version of the 8-bit address 
 * from the datasheet. For example, if an I2C scan detects 0x44, the actual 
 * PT2258 address is 0x88.
 */

#include <PT2258.h>
#include <Wire.h>
#include <IRremote.hpp>

// IR Remote Button Codes
#define IR_RECEIVE_PIN D5 // GPIO14 (D5 on NodeMCU)
const uint32_t VOL_UP = 0xE51ABF40;
const uint32_t VOL_DOWN = 0xE11EBF40;
const uint32_t MUTE = 0xEF10BF40;

// PT2258 I2C Address
PT2258 audioController(0x80); // USE 8-bit address not 7-bit;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("Initializing...");

    // Initialize I2C
    Wire.begin();  // SDA: D2 (GPIO4), SCL: D1 (GPIO5)
    Wire.setClock(100000); // *Important as PT2258 works at clock speed of 100KHz
    
    // Initialize IR Receiver
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    // Check I2C connection
    delay(1000);
    Wire.beginTransmission(0x40);
    if (Wire.endTransmission() == 0) {
        Serial.println("✅ PT2258 detected at 0x40!");
    } else {
        Serial.println("❌ PT2258 not detected at 0x40.");
    }

    // Initialize PT2258
    while (!audioController.begin()) {
        Serial.println("PT2258 connection error! Retrying...");
        delay(1000);
    }

    // Set initial volume and unmute
    audioController.volumeAll(0); // Start with volume at 0
    audioController.mute(false);  // Unmute device
    Serial.println("PT2258 Initialized. Enter volume level (0 to 100):");
}

void loop() {
    static int current_volume = 0;
    static bool is_muted = false;

    // Check IR remote input
    if (IrReceiver.available()) {
        IrReceiver.decode();
        uint32_t received = IrReceiver.decodedIRData.decodedRawData;
        IrReceiver.resume();

        // Handle volume control via IR
        if (received == VOL_UP && current_volume < 100) {
            current_volume = min(current_volume + 5, 100);
            audioController.volumeAll(current_volume);
            Serial.print("Volume Up: ");
            Serial.println(current_volume);
        } else if (received == VOL_DOWN && current_volume > 0) {
            current_volume = max(current_volume - 5, 0);
            audioController.volumeAll(current_volume);
            Serial.print("Volume Down: ");
            Serial.println(current_volume);
        } else if (received == MUTE) {
            is_muted = !is_muted;
            audioController.mute(is_muted);
            Serial.println(is_muted ? "Muted" : "Unmuted");
        }
    }

    // Check Serial input for manual volume control
    if (Serial.available()) {
        int volumeLevel = Serial.parseInt();
        if (volumeLevel >= 0 && volumeLevel <= 100) {
            current_volume = volumeLevel;
            audioController.volumeAll(current_volume);
            Serial.print("Volume Set: ");
            Serial.println(current_volume);
        } else {
            Serial.println("Invalid input! Enter a value between 0 and 100.");
        }

        // Clear Serial buffer
        while (Serial.available()) {
            Serial.read();
        }
    }
}
