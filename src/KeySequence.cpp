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

KeySequence::KeySequence() {
  defaultDelay = 100;
  debug = false;
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
  if(debug) {
    Serial.begin(9600);
  }
}

bool KeySequence::isDebugEnabled() {
  return debug;
}

void KeySequence::sendSequence(String sequence) {
  sendSequenceWithDelay(sequence, defaultDelay);
}

void KeySequence::pressKey(char key) {
  Keyboard.press(key);
  delay(KEY_PRESS_DELAY);
  delay(KEY_RELEASE_DELAY);
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

  // Gestione RELEASE
  if (specialKey == "RELEASE") {
    Keyboard.releaseAll();
    pressedCount = 0;
    return true;
  }

  if(specialKey.startsWith("DELAY")) {
    String delayValue = specialKey.substring(5);
    
    if(delayValue.length() == 0) {
        delay(0);
        return true;
    }
    
    for(unsigned int i = 0; i < delayValue.length(); i++) {
        if(!isDigit(delayValue.charAt(i))) {
            if(debug) {
                Serial.println((__FlashStringHelper*)ERR_INVALID_DELAY);
            }
            return false;
        }
    }
    
    int delayTime = delayValue.toInt();
    if(delayTime > MAX_DELAY_VALUE) {
        if(debug) Serial.println((__FlashStringHelper*)ERR_DELAY_TOO_LARGE);
        return false;
    }
    delay(delayTime);
    return true;
  }
  
  // Modifier keys
  if (specialKey == "ALT") {
    pressedKeys[pressedCount++] = KEY_LEFT_ALT;
    Keyboard.press(KEY_LEFT_ALT);
  }
  else if (specialKey == "RALT") {
    pressedKeys[pressedCount++] = KEY_RIGHT_ALT;
    Keyboard.press(KEY_RIGHT_ALT);
  }
  else if (specialKey == "CTRL") {
    pressedKeys[pressedCount++] = KEY_LEFT_CTRL;
    Keyboard.press(KEY_LEFT_CTRL);
  }
  else if (specialKey == "RCTRL") {
    pressedKeys[pressedCount++] = KEY_RIGHT_CTRL;
    Keyboard.press(KEY_RIGHT_CTRL);
  }
  else if (specialKey == "SHIFT") {
    pressedKeys[pressedCount++] = KEY_LEFT_SHIFT;
    Keyboard.press(KEY_LEFT_SHIFT);
  }
  else if (specialKey == "RSHIFT") {
    pressedKeys[pressedCount++] = KEY_RIGHT_SHIFT;
    Keyboard.press(KEY_RIGHT_SHIFT);
  }
  else if (specialKey == "GUI" || specialKey == "WIN") {
    pressedKeys[pressedCount++] = KEY_LEFT_GUI;
    Keyboard.press(KEY_LEFT_GUI);
  }
  else if (specialKey == "RGUI" || specialKey == "RWIN") {
    pressedKeys[pressedCount++] = KEY_RIGHT_GUI;
    Keyboard.press(KEY_RIGHT_GUI);
  }
  else if (specialKey == "UP") {
    pressedKeys[pressedCount++] = KEY_UP_ARROW;
    Keyboard.press(KEY_UP_ARROW);
  }
  else if (specialKey == "DOWN") {
    pressedKeys[pressedCount++] = KEY_DOWN_ARROW;
    Keyboard.press(KEY_DOWN_ARROW);
  }
  else if (specialKey == "LEFT") {
    pressedKeys[pressedCount++] = KEY_LEFT_ARROW;
    Keyboard.press(KEY_LEFT_ARROW);
  }
  else if (specialKey == "RIGHT") {
    pressedKeys[pressedCount++] = KEY_RIGHT_ARROW;
    Keyboard.press(KEY_RIGHT_ARROW);
  }
  else if (specialKey == "HOME") {
    pressedKeys[pressedCount++] = KEY_HOME;
    Keyboard.press(KEY_HOME);
  }
  else if (specialKey == "END") {
    pressedKeys[pressedCount++] = KEY_END;
    Keyboard.press(KEY_END);
  }
  else if (specialKey == "PAGEUP" || specialKey == "PGUP") {
    pressedKeys[pressedCount++] = KEY_PAGE_UP;
    Keyboard.press(KEY_PAGE_UP);
  }
  else if (specialKey == "PAGEDOWN" || specialKey == "PGDN") {
    pressedKeys[pressedCount++] = KEY_PAGE_DOWN;
    Keyboard.press(KEY_PAGE_DOWN);
  }
  else if (specialKey == "ENTER" || specialKey == "RETURN") {
    pressedKeys[pressedCount++] = KEY_RETURN;
    Keyboard.press(KEY_RETURN);
  }
  else if (specialKey == "TAB") {
    pressedKeys[pressedCount++] = KEY_TAB;
    Keyboard.press(KEY_TAB);
  }
  else if (specialKey == "BACKSPACE" || specialKey == "BKSP") {
    pressedKeys[pressedCount++] = KEY_BACKSPACE;
    Keyboard.press(KEY_BACKSPACE);
  }
  else if (specialKey == "DELETE" || specialKey == "DEL") {
    pressedKeys[pressedCount++] = KEY_DELETE;
    Keyboard.press(KEY_DELETE);
  }
  else if (specialKey == "INSERT" || specialKey == "INS") {
    pressedKeys[pressedCount++] = KEY_INSERT;
    Keyboard.press(KEY_INSERT);
  }
  else if (specialKey == "ESC") {
    pressedKeys[pressedCount++] = KEY_ESC;
    Keyboard.press(KEY_ESC);
  }
  else if (specialKey == "F1") {
    pressedKeys[pressedCount++] = KEY_F1;
    Keyboard.press(KEY_F1);
  }
  else if (specialKey == "F2") {
    pressedKeys[pressedCount++] = KEY_F2;
    Keyboard.press(KEY_F2);
  }
  else if (specialKey == "F3") {
    pressedKeys[pressedCount++] = KEY_F3;
    Keyboard.press(KEY_F3);
  }
  else if (specialKey == "F4") {
    pressedKeys[pressedCount++] = KEY_F4;
    Keyboard.press(KEY_F4);
  }
  else if (specialKey == "F5") {
    pressedKeys[pressedCount++] = KEY_F5;
    Keyboard.press(KEY_F5);
  }
  else if (specialKey == "F6") {
    pressedKeys[pressedCount++] = KEY_F6;
    Keyboard.press(KEY_F6);
  }
  else if (specialKey == "F7") {
    pressedKeys[pressedCount++] = KEY_F7;
    Keyboard.press(KEY_F7);
  }
  else if (specialKey == "F8") {
    pressedKeys[pressedCount++] = KEY_F8;
    Keyboard.press(KEY_F8);
  }
  else if (specialKey == "F9") {
    pressedKeys[pressedCount++] = KEY_F9;
    Keyboard.press(KEY_F9);
  }
  else if (specialKey == "F10") {
    pressedKeys[pressedCount++] = KEY_F10;
    Keyboard.press(KEY_F10);
  }
  else if (specialKey == "F11") {
    pressedKeys[pressedCount++] = KEY_F11;
    Keyboard.press(KEY_F11);
  }
  else if (specialKey == "F12") {
   pressedKeys[pressedCount++] = KEY_F12;
   Keyboard.press(KEY_F12);
 }
 else if (specialKey == "SPACE") {
   pressedKeys[pressedCount++] = ' ';
   Keyboard.press(' ');
 }
 else if (specialKey == "PRINTSCREEN" || specialKey == "PRTSC") {
   pressedKeys[pressedCount++] = 206;
   Keyboard.press(206);
 }
 else if (specialKey == "SCROLLLOCK" || specialKey == "SCRLK") {
   pressedKeys[pressedCount++] = KEY_SCROLL_LOCK;
   Keyboard.press(KEY_SCROLL_LOCK);
 }
 else if (specialKey == "PAUSE" || specialKey == "BREAK") {
   pressedKeys[pressedCount++] = KEY_PAUSE;
   Keyboard.press(KEY_PAUSE);
 }
 else if (specialKey == "NUMLOCK" || specialKey == "NUMLK") {
   pressedKeys[pressedCount++] = KEY_NUM_LOCK;
   Keyboard.press(KEY_NUM_LOCK);
 }
 else if (specialKey == "CAPSLOCK" || specialKey == "CAPS") {
   pressedKeys[pressedCount++] = KEY_CAPS_LOCK;
   Keyboard.press(KEY_CAPS_LOCK);
 }
 else if (specialKey == "MENU" || specialKey == "APP") {
   pressedKeys[pressedCount++] = KEY_MENU;
   Keyboard.press(KEY_MENU);
 }
 else {
   if(debug) {
     Serial.print((__FlashStringHelper*)ERR_UNRECOGNIZED_KEY);
     Serial.println(specialKey);
   }
   return false;
 }
 return true;
}

void KeySequence::sendSequenceWithDelay(String sequence, int delayTime) {
 if(!validateSequence(sequence)) {
   if(debug) Serial.println((__FlashStringHelper*)ERR_INVALID_SEQUENCE);
   return;
 }
 
 bool inSpecialKey = false;
 String specialKey = "";
 pressedCount = 0;  // Reset contatore all'inizio della sequenza
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
         pressKey(specialKey.charAt(j));  // Usa pressKey invece di press+release
       }
     }
     specialKey = "";
     continue;
   }
   
   if (inSpecialKey) {
     specialKey += currentChar;
   }
   else {
     pressKey(currentChar);  // Usa pressKey per i caratteri normali
   }
 }
 
 if (inSpecialKey && debug) {
   Serial.println((__FlashStringHelper*)ERR_INCOMPLETE_SEQUENCE);
 }
 
 delay(delayTime);
 Keyboard.releaseAll();  // Release solo alla fine della sequenza
 pressedCount = 0;       // Reset contatore
}
