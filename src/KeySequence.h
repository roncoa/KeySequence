/**********************
  KeySequence.h
  v 1.2.1
  by roncoa@gmail.com
  20/01/2025
***********************/

#ifndef KeySequence_h
#define KeySequence_h

#ifdef ARDUINO_ARCH_ESP32
#include "USB.h"
#include "USBHIDKeyboard.h"
extern USBHIDKeyboard Keyboard;
#else
#include "Arduino.h"
#include "Keyboard.h"
#endif

#include <stdint.h>
#include "Arduino.h"

class KeySequence {
  private:
    // Struttura per mappare i tasti
    struct KeyMapping {
      const char* name;
      const char* alt_name;
      uint8_t keyCode;
    };
    // Mappature statiche dei tasti
    static const KeyMapping keyMappings[];
    static const int NUM_MAPPINGS;
    // Costanti ottimizzate per la memoria
    static const unsigned int KEY_PRESS_DELAY = 20;  // Aumentato da 5 a 20
    static const int BUFFER_SIZE = 5;
    static const unsigned int MAX_SEQUENCE_LENGTH = 128;
    static const unsigned int MAX_SPECIAL_KEY_LENGTH = 10;
    static const unsigned int MAX_DELAY_VALUE = 10000;
    // Messaggi di errore in flash memory
    static const char ERR_SEQ_TOO_LONG[] PROGMEM;
    static const char ERR_NESTED_BRACKET[] PROGMEM;
    static const char ERR_UNOPENED_BRACKET[] PROGMEM;
    static const char ERR_INVALID_DELAY[] PROGMEM;
    static const char ERR_DELAY_TOO_LARGE[] PROGMEM;
    static const char ERR_SPECIAL_TOO_LONG[] PROGMEM;
    static const char ERR_UNBALANCED_BRACKETS[] PROGMEM;
    static const char ERR_BUFFER_FULL[] PROGMEM;
    static const char ERR_INVALID_SEQUENCE[] PROGMEM;
    static const char ERR_INCOMPLETE_SEQUENCE[] PROGMEM;
    static const char ERR_UNRECOGNIZED_KEY[] PROGMEM;
    // Variabili membro
    uint8_t pressedKeys[BUFFER_SIZE];
    int pressedCount;
    int defaultDelay;
    bool debug;
    bool autoRelease;
    // Metodi helper privati
    bool processDelayCommand(const char* specialKey);
    bool processFunctionKey(const char* specialKey);
    bool processKeyMapping(const char* specialKey);
    bool isValidDelayValue(const char* delayValue);
    void pressKey(char key);
    bool processSpecialKey(const char* specialKey);
    void printDebug(const __FlashStringHelper* message);
    void printDebug(const __FlashStringHelper* message, const char* value);
    bool isModifierKey(uint8_t keyCode);
    void pressAndReleaseKey(uint8_t keyCode);
  public:
    KeySequence();
    void begin();
    void sendSequence(const char* sequence);
    void sendSequence(const String& sequence) { sendSequence(sequence.c_str()); }
    void sendSequenceWithDelay(const char* sequence, int delayTime);
    void sendSequenceWithDelay(const String& sequence, int delayTime) { sendSequenceWithDelay(sequence.c_str(), delayTime); }
    void releaseAll();
    void setDefaultDelay(int delay);
    int getDefaultDelay();
    void setDebug(bool enabled);
    bool isDebugEnabled();
    bool validateSequence(const char* sequence);
    bool validateSequence(const String& sequence) { return validateSequence(sequence.c_str()); }
    void setAutoRelease(bool enabled);
    bool isAutoReleaseEnabled();
};

#endif
