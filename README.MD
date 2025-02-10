# KeySequence Library Documentation

[![GitHub release](https://img.shields.io/github/release/roncoa/KeySequence.svg)](https://github.com/roncoa/KeySequence/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/roncoa/KeySequence.svg?style=social&label=Star)](https://github.com/roncoa/KeySequence/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/roncoa/KeySequence.svg?style=social&label=Fork)](https://github.com/roncoa/KeySequence/network)

If you like **SimRacingController Library**... buy me a beer  [![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/roncoa) 

## Overview
KeySequence is an Arduino/ESP32 library for sending complex key sequences, including special keys, delays, and modifiers. It supports all standard keyboard keys, function keys, and modifier keys with advanced control over key press and release timing.

## Features
- Support for all USB HID platforms (Arduino and ESP32)
- Standard keyboard, special, and modifier keys (CTRL, ALT, SHIFT, etc.)
- Left/right variants of modifier keys
- Customizable delays
- Explicit key release control
- Buffer management for simultaneous key presses
- Input validation
- Debug mode with serial output
- Efficient error messages stored in PROGMEM
- Automatic USB initialization for all platforms

## Installation
### Arduino IDE Library Manager  
You can install directly in the Arduino IDE using the Library Manager. Look under the menu 'Sketch -> Include Library -> Manage Libraries...' to open the Library Manager, then enter 'KeySequence' in the search field and it should find this project and you can click INSTALL.
### Manual via ZIP file
You can use the GitHub 'Download ZIP' feature to get an installable "library" for use with the Arduino IDE. Select 'Sketch -> Include Library -> Add .ZIP Libary' from the Arduino IDE 2.x and select the zip file you downloaded from GitHub, then select open.

## Hardware Requirements
- Arduino board with USB keyboard support (e.g., Leonardo, Micro, Pro Micro)
- ESP32 with USB support (e.g., ESP32-S3)
- USB connection to host computer

## Basic Usage

### Initialization
```cpp
#include <KeySequence.h>

KeySequence keys;

void setup() {
  keys.begin();  // Automatically handles initialization for Arduino and ESP32
}
```

### Simple Sequences
```cpp
// Send text
keys.sendSequence("Hello World");

// Send special keys
keys.sendSequence("{ENTER}");
keys.sendSequence("{TAB}");

// Combinations
keys.sendSequence("{CTRL}c");  // CTRL+C
```

## Special Commands

### Modifier Keys
- `{ALT}`, `{RALT}`
- `{CTRL}`, `{RCTRL}`
- `{SHIFT}`, `{RSHIFT}`
- `{GUI}` or `{WIN}`, `{RGUI}` or `{RWIN}`

### Navigation Keys
- `{UP}`, `{DOWN}`, `{LEFT}`, `{RIGHT}`
- `{HOME}`, `{END}`
- `{PAGEUP}` or `{PGUP}`
- `{PAGEDOWN}` or `{PGDN}`

### Editing Keys
- `{ENTER}` or `{RETURN}`
- `{TAB}`
- `{BACKSPACE}` or `{BKSP}`
- `{DELETE}` or `{DEL}`
- `{INSERT}` or `{INS}`
- `{ESC}`

### Function Keys
- From `{F1}` to `{F12}`

### Special Keys
- `{SPACE}`
- `{PRINTSCREEN}` or `{PRTSC}`
- `{SCROLLLOCK}` or `{SCRLK}`
- `{PAUSE}` or `{BREAK}`
- `{NUMLOCK}` or `{NUMLK}`
- `{CAPSLOCK}` or `{CAPS}`
- `{MENU}` or `{APP}`

### Control Commands
- `{DELAY}` or `{DELAY0}`: No delay
- `{DELAY500}`: Wait for 500ms
- `{RELEASE}`: Release all pressed keys

## Advanced Features

### Key Release Behavior
By default, keys remain pressed until one of these events:
- End of sequence
- Explicit `{RELEASE}` command
- Start of a new sequence

This allows complex combinations like:
```cpp
// CTRL stays pressed for both c and v
keys.sendSequence("{CTRL}cv");

// CTRL is pressed only for c
keys.sendSequence("{CTRL}c{RELEASE}v");
```

### Auto-Release Control
You can control whether keys are automatically released at the end of each sequence:
```cpp
// Disable automatic release at sequence end
keys.setAutoRelease(false);

// Re-enable automatic release (default)
keys.setAutoRelease(true);

// Check current status
bool isAutoRelease = keys.isAutoReleaseEnabled();
```

By default, autoRelease is enabled (true) to maintain compatibility with previous versions.

### Delays
```cpp
// Set default delay between sequences
keys.setDefaultDelay(200);

// Use inline delays
keys.sendSequence("First{DELAY500}Second");

// Send with specific delay
keys.sendSequenceWithDelay("Test", 1000);
```

### Debug Mode
Debug mode provides detailed feedback through the Serial interface:
```cpp
// Enable debug messages (also initializes Serial)
keys.setDebug(true);

// Check if debug is enabled
bool isDebug = keys.isDebugEnabled();
```

Debug messages include:
- Validation errors
- Unrecognized keys
- Buffer overflow warnings
- Sequence parsing errors

### Sequence Validation
You can validate sequences before sending them:
```cpp
if(keys.validateSequence("{CTRL}c")) {
  // Sequence is valid
  keys.sendSequence("{CTRL}c");
}
```

The validator checks:
- Sequence length limits
- Balanced curly braces
- Valid special keys
- Valid delay values

## Technical Specifications

### Constants
```cpp
static const unsigned int KEY_PRESS_DELAY = 20;        // ms between presses
static const int BUFFER_SIZE = 5;                      // max simultaneous keys
static const unsigned int MAX_SEQUENCE_LENGTH = 128;   // max sequence characters
static const unsigned int MAX_SPECIAL_KEY_LENGTH = 10; // max special key length
static const unsigned int MAX_DELAY_VALUE = 10000;     // max delay ms
```

### Memory Usage
- Error messages stored in PROGMEM
- Dynamic buffer for pressed keys
- Static string validation
- Minimal RAM footprint

### Timing
- 20ms minimum between key press/release (optimized for ESP32)
- 100ms default between sequences
- Custom delays up to 10 seconds
- Precise timing using Arduino's `delay()`

## Usage Examples

### Text Editor Commands
```cpp
// Select all
keys.sendSequence("{CTRL}a");

// Copy-paste with explicit release
keys.sendSequence("{CTRL}c{RELEASE}{CTRL}v");

// Save file
keys.sendSequence("{CTRL}s");
```

### Window Management
```cpp
// Alt-Tab window switch
keys.sendSequence("{ALT}{TAB}");

// Open Run dialog
keys.sendSequence("{GUI}r");

// Task Manager
keys.sendSequence("{CTRL}{SHIFT}{ESC}");
```

### Game Macros
```cpp
// Press multiple keys
keys.sendSequence("{SHIFT}{W}");

// Quick actions with delays
keys.sendSequence("1{DELAY200}2{DELAY200}3");

// Complex combinations
keys.sendSequence("{CTRL}{SPACE}{DELAY100}e");
```

## Error Handling

### Common Errors
1. **Sequence Too Long**
   ```cpp
   // Will fail validation if > 128 characters
   String longSequence = "...";
   ```

2. **Buffer Overflow**
   ```cpp
   // Too many simultaneous keys (max 5)
   keys.sendSequence("{CTRL}{ALT}{SHIFT}{WIN}{F1}{F2}");
   ```

3. **Invalid Braces**
   ```cpp
   // Missing closing brace
   keys.sendSequence("{CTRL");
   
   // Missing opening brace
   keys.sendSequence("CTRL}");
   ```

### Debug Output Examples
```
Error: sequence too long
Error: unbalanced curly braces
Error: key buffer full
Error: non-numeric delay value
Special key not recognized: XYZ
```

## Platform-Specific Notes

### Arduino Leonardo/Micro
- Native compatibility with Arduino Keyboard library
- Optimal timing for most applications
- Full support for all features

### ESP32 (S3)
- Native USB HID support
- Automatic USB initialization
- Optimized timing (20ms KEY_PRESS_DELAY)
- Same API as Arduino for maximum compatibility

## Project Structure
```
KeySequence/
│
├── examples/
│   └── Basic/
│       └── Basic.ino          // Working example for all platforms
│
├── src/
│   ├── KeySequence.h
│   └── KeySequence.cpp
│
├── keywords.txt
└── library.properties
```

## Version History

### v1.3.0 (Current Version)
- BugFix ESP32 support (tested on ESP32-S3)

### v1.2.1
- BugFix pressKey
- Executes releaseAll() before ENTER

### v1.2.0
- Added ESP32 support (tested on ESP32-S3)
- Automatic USB initialization for ESP32
- Increased default KEY_PRESS_DELAY to 20ms for better compatibility
- Unified codebase for Arduino and ESP32
- Memory constants optimization
- Reduced BUFFER_SIZE to 5 to optimize RAM usage
- Reduced MAX_SEQUENCE_LENGTH to 128 characters
- Reduced MAX_SPECIAL_KEY_LENGTH to 10 characters

### v1.1.1
- Bug fixes, various improvements

### v1.1.0
- Added configurable key auto-release control
- New setAutoRelease() and isAutoReleaseEnabled() methods
- Backward-compatible behavior (auto-release enabled by default)

### v1.0.0
- Initial release
- Basic sequence support
- Special key handling
- Delay control
- Debug functionality

### Planned Features
- Multiple keyboard layouts
- Macro recording
- Sequence chaining
- Custom key definitions
- Improved error handling

## How to Contribute
1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License
This library is released under the MIT License. See LICENSE file for details.

## Support
For support, please:
1. Check the documentation
2. Enable debug mode
3. Search existing issues
4. Create a new issue if needed
5. Contact maintainers

## FAQ

### General Questions

**Q: Which boards are supported?**
A: All boards with native USB HID support:
- Arduino (Leonardo, Micro, Pro Micro)
- ESP32 with USB support (like ESP32-S3)

**Q: What is the maximum sequence length?**
A: 128 characters

**Q: How many keys can be pressed simultaneously?**
A: 5 keys (BUFFER_SIZE)

### Technical Questions

**Q: Why aren't my key combinations working?**
A: Check:
1. Key release points
2. Buffer size limits (max 5 keys)
3. Timing/delays (20ms minimum)
4. Host system support

**Q: How do I add custom keys?**
A: Modify the processSpecialKey method in KeySequence.cpp

**Q: Are there differences between Arduino and ESP32?**
A: No, the library automatically handles platform differences. The same code works on both without modifications.
