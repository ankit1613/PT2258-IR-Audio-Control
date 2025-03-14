# PT2258-IR-Audio-Control

## Overview
I have made this project as a sb project of some other proect. Hope it might be helpful to others also.
It is designed to control the PT2258 6-channel electronic volume controller using an IR remote and I2C communication. The system enables volume adjustment, muting (and also attenuation control is you need) via an ESP8266 microcontroller. By integrating an IR receiver, users can remotely modify audio settings with pre-defined IR codes.

## Features
- **IR Remote Control**: Adjust volume, mute, and attenuation using an infrared remote.
- **I2C Communication**: Uses the PT2258 audio controller IC for precise volume control.
- **ESP8266 Integration**: Leverages the NodeMCU board for seamless control.
- **Customizable Settings**: Modify volume levels and attenuation parameters programmatically.

## Hardware Requirements
- PT2258 Audio Volume Controller IC
- IR Receiver (e.g., TSOP1838, connected to GPIO14 / D5)
- NodeMCU (ESP8266)
- Level shifter (must 3.3v to 5V)
- I2C Connections:
  - SDA: GPIO4 / D2
  - SCL: GPIO5 / D1

## PT2258 I2C Address Configuration
The PT2258 volume controller IC has configurable I2C addresses based on the CODE1 and CODE2 pin connections. The corresponding addresses are:

| CODE1 | CODE2 | 8-bit Address | 7-bit Address (for Wire library) |
|-------|-------|--------------|----------------------------------|
|   0   |   0   |    0x80      |    0x40                         |
|   1   |   0   |    0x88      |    0x44                         |
|   0   |   1   |    0x84      |    0x42                         |
|   1   |   1   |    0x8C      |    0x46                         |

The Wire library uses a 8-bit address format, so when performing an I2C scan, the detected address appears as a right-shifted version of the 8-bit address specified in the datasheet. For example, if an I2C scan detects 0x44, the actual PT2258 address is 0x88.

## IR Remote Button Codes
| Function       | IR Code      |
|---------------|-------------|
| Volume Up     | 0xE51ABF40  |
| Volume Down   | 0xE11EBF40  |
| Mute Toggle   | 0xEF10BF40  |

(These were ir codes in my case you may find yours from by the code for finding ir codes I provided)

## Software Setup
1. Install the required Arduino libraries:
   - `Wire.h` for I2C communication
   - `IRremote.hpp` for infrared signal processing
   - `PT2258.h` for handling the volume controller
2. Upload the provided firmware to the ESP8266.
3. Connect the hardware components as specified above.
4. Use the IR remote to adjust volume or send serial commands for manual control.

## Usage
- Press the volume up/down buttons to adjust the volume.
- Press the mute button to toggle mute.
- Press attenuation up/down buttons to fine-tune audio attenuation.
- Optionally, enter volume values via the serial monitor.

## Future Enhancements
- I needed this project as I didn't have volume comtrol in a monitor which i converted to led.
- In future I will compeletely integrate it with that project including fabricating a pcb.

---
This project is open-source, and contributions are welcome!

