/**********************
  KeySequence.cpp
  v 1.3.0
  by roncoa@gmail.com
  20/01/2025
***********************/

#include "KeySequence.h"

// Definizione messaggi di errore in flash memory
const char KeySequence::ERR_SEQ_TOO_LONG[] PROGMEM = "Seq too long";
const char KeySequence::ERR_NESTED_BRACKET[] PROGMEM = "Nested bracket";
const char KeySequence::ERR_UNOPENED_BRACKET[] PROGMEM = "Unopened bracket";
const char KeySequence::ERR_INVALID_DELAY[] PROGMEM = "Invalid delay";
const char KeySequence::ERR_DELAY_TOO_LARGE[] PROGMEM = "Delay too large";
const char KeySequence::ERR_SPECIAL_TOO_LONG[] PROGMEM = "Special too long";
const char KeySequence::ERR_UNBALANCED_BRACKETS[] PROGMEM = "Unbalanced brackets";
const char KeySequence::ERR_BUFFER_FULL[] PROGMEM = "Buffer full";
const char KeySequence::ERR_INVALID_SEQUENCE[] PROGMEM = "Invalid seq";
const char KeySequence::ERR_INCOMPLETE_SEQUENCE[] PROGMEM = "Incomplete seq";
const char KeySequence::ERR_UNRECOGNIZED_KEY[] PROGMEM = "Unknown key:";

// Definizione delle mappature statiche
const KeySequence::KeyMapping KeySequence::keyMappings[] = {
  {"SHIFT", nullptr, KEY_LEFT_SHIFT},
  {"RSHIFT", nullptr, KEY_RIGHT_SHIFT},
  {"ALT", nullptr, KEY_LEFT_ALT},
  {"RALT", nullptr, KEY_RIGHT_ALT},
  {"CTRL", nullptr, KEY_LEFT_CTRL},
  {"RCTRL", nullptr, KEY_RIGHT_CTRL},
  {"GUI", "WIN", KEY_LEFT_GUI},
  {"RGUI", "RWIN", KEY_RIGHT_GUI},
  {"ENTER", "RETURN", KEY_RETURN},
  {"ESC", nullptr, KEY_ESC},
  {"BACKSPACE", "BKSP", KEY_BACKSPACE},
  {"TAB", nullptr, KEY_TAB},
  {"SPACE", nullptr, ' '},
  {"UP", nullptr, KEY_UP_ARROW},
  {"DOWN", nullptr, KEY_DOWN_ARROW},
  {"LEFT", nullptr, KEY_LEFT_ARROW},
  {"RIGHT", nullptr, KEY_RIGHT_ARROW},
  {"INSERT", "INS", KEY_INSERT},
  {"DELETE", "DEL", KEY_DELETE},
  {"HOME", nullptr, KEY_HOME},
  {"END", nullptr, KEY_END},
  {"PAGEUP", "PGUP", KEY_PAGE_UP},
  {"PAGEDOWN", "PGDN", KEY_PAGE_DOWN},
  {"PRINTSCREEN", "PRTSC", 206},
  {"SCROLLLOCK", "SCRLK", KEY_SCROLL_LOCK},
  {"PAUSE", "BREAK", KEY_PAUSE},
  {"NUMLOCK", "NUMLK", KEY_NUM_LOCK},
  {"CAPSLOCK", "CAPS", KEY_CAPS_LOCK},
  {"MENU", "APP", KEY_MENU}
};

const int KeySequence::NUM_MAPPINGS = sizeof(keyMappings) / sizeof(KeyMapping);

void KeySequence::printDebug(const __FlashStringHelper* message) {
  if (debug) {
    Serial.println(message);
    Serial.flush();
  }
}

void KeySequence::printDebug(const __FlashStringHelper* message, const char* value) {
  if (debug) {
    Serial.print(message);
    Serial.print(F(" '"));
    Serial.print(value);
    Serial.println(F("'"));
    Serial.flush();
  }
}

KeySequence::KeySequence() {
  defaultDelay = 100;
  debug = false;
  autoRelease = true;
  pressedCount = 0;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    pressedKeys[i] = 0;
  }
}

void KeySequence::begin() {
#ifdef ARDUINO_ARCH_ESP32
  if (!USB.begin()) {
    if (debug) Serial.println(F("USB.begin() failed"));
    return;
  }
  USB.usbClass(TUSB_CLASS_HID);
  USB.usbSubClass(0);
  USB.usbProtocol(0);
  USB.manufacturerName("SimRacing");
  USB.productName("KeySequence");
  Keyboard.begin();
#else
  Keyboard.begin();
#endif
}

void KeySequence::releaseAll() {
  if (debug) Serial.println(F("Releasing all keys"));
#ifdef ARDUINO_ARCH_ESP32
  for (int i = 0; i < pressedCount; i++) {
    Keyboard.release(pressedKeys[i]);
  }
#else
  Keyboard.releaseAll();
#endif
  pressedCount = 0;
}

void KeySequence::setDefaultDelay(int delay) {
  defaultDelay = delay;
}

int KeySequence::getDefaultDelay() {
  return defaultDelay;
}

void KeySequence::setDebug(bool enabled) {
  debug = enabled;
}

bool KeySequence::isDebugEnabled() {
  return debug;
}

void KeySequence::setAutoRelease(bool enabled) {
  autoRelease = enabled;
}

bool KeySequence::isAutoReleaseEnabled() {
  return autoRelease;
}

bool KeySequence::isModifierKey(uint8_t keyCode) {
  return keyCode == KEY_LEFT_SHIFT || keyCode == KEY_RIGHT_SHIFT ||
         keyCode == KEY_LEFT_CTRL || keyCode == KEY_RIGHT_CTRL ||
         keyCode == KEY_LEFT_ALT || keyCode == KEY_RIGHT_ALT ||
         keyCode == KEY_LEFT_GUI || keyCode == KEY_RIGHT_GUI;
}

void KeySequence::pressKey(char key) {
  if (debug) {
    Serial.print(F("Pressing regular key: '"));
    Serial.write(key);
    Serial.println(F("'"));
  }
#ifdef ARDUINO_ARCH_ESP32
  Keyboard.write(key);
#else
  Keyboard.press(key);
#endif
  delay(KEY_PRESS_DELAY);
  //Keyboard.release(key);
  //delay(KEY_PRESS_DELAY);
}

void KeySequence::pressAndReleaseKey(uint8_t keyCode) {
  if (debug) {
    Serial.print(F("Pressing and releasing key code: "));
    Serial.println(keyCode);
  }
#ifdef ARDUINO_ARCH_ESP32
  Keyboard.press(keyCode);
#else
  Keyboard.press(keyCode);
#endif
  delay(KEY_PRESS_DELAY);
  Keyboard.release(keyCode);
  delay(KEY_PRESS_DELAY);
}

bool KeySequence::isValidDelayValue(const char* delayValue) {
  if (!delayValue || !*delayValue) {
    printDebug((__FlashStringHelper*)ERR_INVALID_DELAY);
    return false;
  }

  for (const char* p = delayValue; *p; p++) {
    if (!isDigit(*p)) {
      printDebug((__FlashStringHelper*)ERR_INVALID_DELAY);
      return false;
    }
  }
  return true;
}

bool KeySequence::processDelayCommand(const char* specialKey) {
  const char* delayValue = specialKey + 5;  // Skip "DELAY" prefix

  if (!*delayValue) {
    delay(0);
    return true;
  }

  if (!isValidDelayValue(delayValue)) {
    return false;
  }

  long delayTime = atol(delayValue);
  if (delayTime > MAX_DELAY_VALUE) {
    printDebug((__FlashStringHelper*)ERR_DELAY_TOO_LARGE);
    return false;
  }

  if (debug) {
    Serial.print(F("Delaying for "));
    Serial.print(delayTime);
    Serial.println(F(" ms"));
    Serial.flush();
  }

  delay(delayTime);
  return true;
}

bool KeySequence::processFunctionKey(const char* specialKey) {
  if (strlen(specialKey) < 2 || specialKey[0] != 'F') return false;

  int num = atoi(specialKey + 1);
  if (num >= 1 && num <= 12) {
    uint8_t keyCode = KEY_F1 + (num - 1);
    if (pressedCount < BUFFER_SIZE) {
      pressedKeys[pressedCount++] = keyCode;
      pressAndReleaseKey(keyCode);
      return true;
    }
  }
  return false;
}

bool KeySequence::processKeyMapping(const char* specialKey) {
  for (int i = 0; i < NUM_MAPPINGS; i++) {
    const KeyMapping& mapping = keyMappings[i];
    if (strcmp(specialKey, mapping.name) == 0 ||
        (mapping.alt_name && strcmp(specialKey, mapping.alt_name) == 0)) {
      if (pressedCount < BUFFER_SIZE) {
        pressedKeys[pressedCount++] = mapping.keyCode;
        if (isModifierKey(mapping.keyCode)) {
          if (debug) Serial.println(F("Pressing modifier key"));
#ifdef ARDUINO_ARCH_ESP32
          Keyboard.press(mapping.keyCode);
#else
          Keyboard.press(mapping.keyCode);
#endif
          delay(KEY_PRESS_DELAY);
        } else {
          if (debug) Serial.println(F("Pressing and releasing regular special key"));
          pressAndReleaseKey(mapping.keyCode);
        }
        return true;
      }
    }
  }
  return false;
}

bool KeySequence::processSpecialKey(const char* specialKey) {
  if (!specialKey || strlen(specialKey) > MAX_SPECIAL_KEY_LENGTH) {
    printDebug(F("Special key too long or null"));
    return false;
  }

  char upperKey[MAX_SPECIAL_KEY_LENGTH + 1] = {0};
  strncpy(upperKey, specialKey, MAX_SPECIAL_KEY_LENGTH);

  if (debug) {
    Serial.print(F("Processing special key: '"));
    Serial.print(upperKey);
    Serial.println(F("'"));
    Serial.flush();
  }

  // Convert to uppercase
  for (char* p = upperKey; *p; p++) {
    *p = toupper(*p);
  }

  if (debug) {
    Serial.print(F("After uppercase: '"));
    Serial.print(upperKey);
    Serial.println(F("'"));
    Serial.flush();
  }

  if (strncmp(upperKey, "DELAY", 5) == 0) {
    if (debug) Serial.println(F("Processing DELAY command"));
    return processDelayCommand(upperKey);
  }

  if (strcmp(upperKey, "RELEASE") == 0) {
    if (debug) Serial.println(F("Processing RELEASE command"));
    releaseAll();
    return true;
  }

  if (processFunctionKey(upperKey)) {
    if (debug) Serial.println(F("Processed as function key"));
    return true;
  }

  if (processKeyMapping(upperKey)) {
    if (debug) Serial.println(F("Processed as standard key"));
    return true;
  }

  if (debug) {
    Serial.print(F("Unrecognized key: '"));
    Serial.print(upperKey);
    Serial.println(F("'"));
    Serial.flush();
  }
  return false;
}

bool KeySequence::validateSequence(const char* sequence) {
  if (!sequence) {
    printDebug(F("Null sequence"));
    return false;
  }

  size_t len = strlen(sequence);
  if (len > MAX_SEQUENCE_LENGTH) {
    printDebug((__FlashStringHelper*)ERR_SEQ_TOO_LONG);
    return false;
  }

  int openBrackets = 0;
  bool inSpecialKey = false;
  char specialKey[MAX_SPECIAL_KEY_LENGTH + 1] = {0};
  int specialKeyLen = 0;

  for (const char* p = sequence; *p; p++) {
    char c = *p;

    if (c == '{') {
      if (inSpecialKey) {
        printDebug((__FlashStringHelper*)ERR_NESTED_BRACKET);
        return false;
      }
      inSpecialKey = true;
      openBrackets++;
      continue;
    }

    if (c == '}') {
      if (!inSpecialKey) {
        printDebug((__FlashStringHelper*)ERR_UNOPENED_BRACKET);
        return false;
      }
      inSpecialKey = false;
      openBrackets--;
      specialKey[specialKeyLen] = '\0';

      if (strncmp(specialKey, "DELAY", 5) == 0) {
        const char* delayValue = specialKey + 5;
        if (*delayValue) {
          char* endPtr;
          long value = strtol(delayValue, &endPtr, 10);
          if (*endPtr != '\0' || value > MAX_DELAY_VALUE) {
            printDebug((__FlashStringHelper*)ERR_INVALID_DELAY);
            return false;
          }
        }
      }
      specialKeyLen = 0;
      continue;
    }

    if (inSpecialKey) {
      if (specialKeyLen >= MAX_SPECIAL_KEY_LENGTH) {
        printDebug((__FlashStringHelper*)ERR_SPECIAL_TOO_LONG);
        return false;
      }
      specialKey[specialKeyLen++] = c;
    }
  }

  if (openBrackets != 0) {
    printDebug((__FlashStringHelper*)ERR_UNBALANCED_BRACKETS);
    return false;
  }

  return true;
}

void KeySequence::sendSequence(const char* sequence) {
  sendSequenceWithDelay(sequence, defaultDelay);
}

void KeySequence::sendSequenceWithDelay(const char* sequence, int delayTime) {
  if (!sequence) {
    printDebug(F("Null sequence received"));
    return;
  }

  if (!validateSequence(sequence)) {
    printDebug((__FlashStringHelper*)ERR_INVALID_SEQUENCE);
    return;
  }

  bool inSpecialKey = false;
  char specialKey[MAX_SPECIAL_KEY_LENGTH + 1] = {0};
  int specialKeyLen = 0;
  pressedCount = 0;
  releaseAll();

  if (debug) {
    Serial.print(F("Processing sequence: '"));
    Serial.print(sequence);
    Serial.println(F("'"));
    Serial.flush();
  }

  for (const char* p = sequence; *p; p++) {
    char currentChar = *p;

    if (currentChar == '{') {
      if (debug) Serial.println(F("Found opening bracket"));
      inSpecialKey = true;
      specialKeyLen = 0;
      continue;
    }

    if (currentChar == '}') {
      if (debug) {
        specialKey[specialKeyLen] = '\0';
        Serial.print(F("Found closing bracket, key: '"));
        Serial.print(specialKey);
        Serial.println(F("'"));
      }
      inSpecialKey = false;
      specialKey[specialKeyLen] = '\0';
      bool recognized = processSpecialKey(specialKey);
      if (!recognized) {
        if (debug) {
          Serial.println(F("Key not recognized, ignoring"));
        }
      }
      specialKeyLen = 0;
      continue;
    }

    if (inSpecialKey) {
      if (specialKeyLen < MAX_SPECIAL_KEY_LENGTH) {
        specialKey[specialKeyLen++] = currentChar;
      }
    } else {
      pressKey(currentChar);
      if (strlen(p) > 1) {
        Keyboard.release(currentChar);
      }
    }
  }

  if (inSpecialKey) {
    printDebug(F("Warning: unclosed special key sequence"));
  }

  delay(delayTime);

  if (autoRelease) {
    if (debug) Serial.println(F("Auto-releasing all keys"));
    releaseAll();
  }
}
