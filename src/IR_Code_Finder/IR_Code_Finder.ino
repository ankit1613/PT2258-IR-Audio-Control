#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN D5 // Use GPIO14 (D5 on NodeMCU)

void setup() {
    Serial.begin(115200);
    delay(1000); // Small delay to allow Serial to initialize

    Serial.println(F("ESP8266 Booted Successfully!"));
    
    // Check if Serial is working
    for (int i = 5; i > 0; i--) {
        Serial.print("Starting IR Receiver in ");
        Serial.print(i);
        Serial.println(" seconds...");
        delay(1000);
    }

    Serial.println(F("Initializing IR Receiver..."));
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    Serial.println(F("IR Receiver Ready!"));
}

void loop() {
    if (IrReceiver.available()) {
        IrReceiver.decode();

        if (IrReceiver.decodedIRData.decodedRawData != 0x0) {
            Serial.print("IR Code HEX: 0x");
            Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        }

        IrReceiver.resume();
    }
}
