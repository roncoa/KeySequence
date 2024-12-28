// KeySequence.h
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

  // Costanti per i limiti
  static const unsigned int KEY_PRESS_DELAY = 5;
  static const int BUFFER_SIZE = 10;
  static const unsigned int MAX_SEQUENCE_LENGTH = 256;
  static const unsigned int MAX_SPECIAL_KEY_LENGTH = 20;
  static const unsigned int MAX_DELAY_VALUE = 10000;

  // Messaggi di errore in memoria programma
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
  bool processDelayCommand(const String& specialKey);
  bool processFunctionKey(const String& specialKey);
  bool processKeyMapping(const String& specialKey);
  bool isValidDelayValue(const String& delayValue);
  void pressKey(char key);
  bool processSpecialKey(String specialKey);

public:
  KeySequence();
  void begin();
  void sendSequence(String sequence);
  void sendSequenceWithDelay(String sequence, int delayTime);
  void setDefaultDelay(int delay);
  int getDefaultDelay();
  void setDebug(bool enabled);
  bool isDebugEnabled();
  bool validateSequence(String sequence);
  void setAutoRelease(bool enabled);
  bool isAutoReleaseEnabled();
};

#endif
