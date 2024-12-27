/*
 * Arduino Leonardo KeySequence Example
 * 
 * Esempio di utilizzo della libreria KeySequence con Arduino Leonardo.
 * Il programma simula diverse funzioni di tastiera in base ai pulsanti premuti.
 * 
 * Hardware:
 * - Arduino Leonardo
 * - 3 pulsanti collegati a pin digitali
 * - LED su pin 13 per feedback
 * 
 * Collegamenti:
 * - Pulsante 1: Pin 2 -> GND  (macro di testo)
 * - Pulsante 2: Pin 3 -> GND  (scorciatoie Windows)
 * - Pulsante 3: Pin 4 -> GND  (combinazioni di tasti)
 * - LED: Pin 13 (built-in LED)
 */

#include <Arduino.h>
#include "Keyboard.h"
#include "KeySequence.h"

KeySequence keys;

// Definizione pin
const int BTN_MACRO = 2;    // Macro di testo
const int BTN_WIN = 3;      // Scorciatoie Windows
const int BTN_COMBO = 4;    // Combinazioni tasti
const int LED_PIN = 13;     // LED di stato

// Variabili per il debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonStates[] = {HIGH, HIGH, HIGH};

void setup() {
  // Inizializza seriale per debug
  Serial.begin(9600);
  while (!Serial) {
    ; // Attendi la porta seriale per Leonardo
  }
  
  // Inizializza la tastiera
  Keyboard.begin();
  keys.begin();
  
  [resto del codice come prima...]
