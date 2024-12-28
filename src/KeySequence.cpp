// KeySequence.cpp
#include "KeySequence.h"

// Definizione messaggi di errore
const char KeySequence::ERR_SEQ_TOO_LONG[] PROGMEM = "Errore: sequenza troppo lunga";
const char KeySequence::ERR_NESTED_BRACKET[] PROGMEM = "Errore: parentesi graffa aperta dentro un comando";
const char KeySequence::ERR_UNOPENED_BRACKET[] PROGMEM = "Errore: parentesi graffa chiusa senza apertura";
const char KeySequence::ERR_INVALID_DELAY[] PROGMEM = "Errore: valore delay non numerico";
const char KeySequence::ERR_DELAY_TOO_LARGE[] PROGMEM = "Errore: valore delay troppo grande";
const char KeySequence::ERR_SPECIAL_TOO_LONG[] PROGMEM = "Errore: comando speciale troppo lungo";
const char KeySequence::ERR_UNBALANCED_BRACKETS[] PROGMEM = "Errore: parentesi graffe non bilanciate";
const char KeySequence::ERR_BUFFER_FULL[] PROGMEM = "Errore: buffer tasti pieno";
const char KeySequence::ERR_INVALID_SEQUENCE[] PROGMEM = "Sequenza non valida";
const char KeySequence::ERR_INCOMPLETE_SEQUENCE[] PROGMEM = "Errore: sequenza termina dentro una chiave speciale";
const char KeySequence::ERR_UNRECOGNIZED_KEY[] PROGMEM = "Tasto speciale non riconosciuto: ";

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

// Costruttore
KeySequence::KeySequence() {
    defaultDelay = 100;
    debug = false;
    autoRelease = true;
    pressedCount = 0;
    for(int i = 0; i < BUFFER_SIZE; i++) {
        pressedKeys[i] = 0;
    }
}

void KeySequence::begin() {
    Keyboard.begin();
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

void KeySequence::pressKey(char key) {
    Keyboard.press(key);
    Keyboard.release(key);
    delay(KEY_PRESS_DELAY);
}

bool KeySequence::isValidDelayValue(const String& delayValue) {
    if(!delayValue.length() || !delayValue.charAt(0)) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_INVALID_DELAY);
        return false;
    }
    
    for(unsigned int i = 0; i < delayValue.length(); i++) {
        if(!isDigit(delayValue.charAt(i))) {
            if(debug) Serial.println((__FlashStringHelper*)ERR_INVALID_DELAY);
            return false;
        }
    }
    return true;
}

bool KeySequence::processDelayCommand(const String& specialKey) {
    String delayValue = specialKey.substring(5);
    
    if(delayValue.length() == 0) {
        delay(0);
        return true;
    }
    
    if(!isValidDelayValue(delayValue)) {
        return false;
    }
    
    int delayTime = delayValue.toInt();
    if(delayTime > MAX_DELAY_VALUE) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_DELAY_TOO_LARGE);
        return false;
    }
    delay(delayTime);
    return true;
}

bool KeySequence::processFunctionKey(const String& specialKey) {
    if(specialKey.length() < 2 || specialKey[0] != 'F') return false;
    
    String numStr = specialKey.substring(1);
    int num = numStr.toInt();
    
    if(num >= 1 && num <= 12) {
        uint8_t keyCode = KEY_F1 + (num-1);
        pressedKeys[pressedCount++] = keyCode;
        Keyboard.press(keyCode);
        return true;
    }
    return false;
}

bool KeySequence::processKeyMapping(const String& specialKey) {
    for(int i = 0; i < NUM_MAPPINGS; i++) {
        const KeyMapping& mapping = keyMappings[i];
        if(specialKey == mapping.name || 
           (mapping.alt_name && specialKey == mapping.alt_name)) {
            pressedKeys[pressedCount++] = mapping.keyCode;
            Keyboard.press(mapping.keyCode);
            return true;
        }
    }
    return false;
}

bool KeySequence::processSpecialKey(String specialKey) {
    if(specialKey.length() > MAX_SPECIAL_KEY_LENGTH) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_SPECIAL_TOO_LONG);
        return false;
    }

    if(pressedCount >= BUFFER_SIZE) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_BUFFER_FULL);
        return false;
    }
    
    specialKey.toUpperCase();

    // Gestione DELAY
    if(specialKey.startsWith("DELAY")) {
        return processDelayCommand(specialKey);
    }

    // Gestione RELEASE
    if(specialKey == "RELEASE") {
        Keyboard.releaseAll();
        pressedCount = 0;
        return true;
    }

    // Gestione tasti funzione
    if(processFunctionKey(specialKey)) {
        return true;
    }

    // Gestione mappature standard
    if(processKeyMapping(specialKey)) {
        return true;
    }

    if(debug) {
        Serial.print((__FlashStringHelper*)ERR_UNRECOGNIZED_KEY);
        Serial.println(specialKey);
    }
    return false;
}

bool KeySequence::validateSequence(String sequence) {
    if(sequence.length() > MAX_SEQUENCE_LENGTH) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_SEQ_TOO_LONG);
        return false;
    }

    int openBrackets = 0;
    bool inSpecialKey = false;
    String currentKey = "";
    
    for(unsigned int i = 0; i < sequence.length(); i++) {
        char c = sequence.charAt(i);
        
        if(c == '{') {
            if(inSpecialKey) {
                if(debug) Serial.println((__FlashStringHelper*)ERR_NESTED_BRACKET);
                return false;
            }
            inSpecialKey = true;
            openBrackets++;
            continue;
        }
        
        if(c == '}') {
            if(!inSpecialKey) {
                if(debug) Serial.println((__FlashStringHelper*)ERR_UNOPENED_BRACKET);
                return false;
            }
            inSpecialKey = false;
            openBrackets--;
            
            if(currentKey.startsWith("DELAY")) {
                String delayValue = currentKey.substring(5);
                if(delayValue.length() > 0) {
                    for(unsigned int j = 0; j < delayValue.length(); j++) {
                        if(!isDigit(delayValue.charAt(j))) {
                            if(debug) Serial.println((__FlashStringHelper*)ERR_INVALID_DELAY);
                            return false;
                        }
                    }
                    if(delayValue.toInt() > MAX_DELAY_VALUE) {
                        if(debug) Serial.println((__FlashStringHelper*)ERR_DELAY_TOO_LARGE);
                        return false;
                    }
                }
            }
            currentKey = "";
            continue;
        }
        
        if(inSpecialKey) {
            currentKey += c;
            if(currentKey.length() > MAX_SPECIAL_KEY_LENGTH) {
                if(debug) Serial.println((__FlashStringHelper*)ERR_SPECIAL_TOO_LONG);
                return false;
            }
        }
    }
    
    if(openBrackets != 0) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_UNBALANCED_BRACKETS);
        return false;
    }
    
    return true;
}

void KeySequence::sendSequence(String sequence) {
    sendSequenceWithDelay(sequence, defaultDelay);
}

void KeySequence::sendSequenceWithDelay(String sequence, int delayTime) {
    if(!validateSequence(sequence)) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_INVALID_SEQUENCE);
        return;
    }
    
    bool inSpecialKey = false;
    String specialKey = "";
    pressedCount = 0;
    Keyboard.releaseAll(); // Reset iniziale dello stato
    
    for (int i = 0; i < sequence.length(); i++) {
        char currentChar = sequence.charAt(i);
        
        if (currentChar == '{') {
            inSpecialKey = true;
            continue;
        }
        
        if (currentChar == '}') {
            inSpecialKey = false;
            bool recognized = processSpecialKey(specialKey);
            if (!recognized) {
                for(unsigned int j = 0; j < specialKey.length(); j++) {
                    pressKey(specialKey.charAt(j));
                }
            }
            specialKey = "";
            continue;
        }
        
        if (inSpecialKey) {
            specialKey += currentChar;
        }
        else {
            pressKey(currentChar);
        }
    }
    
    if (inSpecialKey && debug) {
        Serial.println((__FlashStringHelper*)ERR_INCOMPLETE_SEQUENCE);
    }
    
    delay(delayTime);
    
    if (autoRelease) {
        Keyboard.releaseAll();
        pressedCount = 0;
    }
}
