/*
 * ESP32-S3 KeySequence Example
 * 
 * Questo esempio mostra come utilizzare la libreria KeySequence
 * su ESP32-S3 per simulare una tastiera USB.
 * 
 * Hardware necessario:
 * - ESP32-S3 (con supporto USB nativo)
 * - Connessione USB al computer
 * 
 * Note: 
 * - Assicurarsi di utilizzare una porta USB che supporti la modalità device
 * - Su alcuni ESP32-S3 potrebbe essere necessario tenere premuto BOOT durante
 *   il caricamento del programma
 */

#include "USB.h"
#include "USBHIDKeyboard.h"
#include "KeySequence.h"

USBHIDKeyboard Keyboard;
KeySequence keys;

// Pin per il pulsante di trigger (esempio con GPIO5)
const int buttonPin = 5;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  // Inizializza USB e HID
  USB.begin();
  Keyboard.begin();
  delay(1000);  // Attendi l'inizializzazione USB
  
  // Inizializza la tastiera KeySequence
  keys.begin();
  
  // Abilita il debug seriale
  Serial.begin(115200);
  keys.setDebug(true);
  
  // Configura il pulsante con pull-up interno
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Imposta un delay di default di 100ms tra le sequenze
  keys.setDefaultDelay(100);
  
  // Disabilita l'auto-release per alcune macro
  keys.setAutoRelease(false);
}

[resto del codice come prima...]
