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

#include "KeySequence.h"

// Istanza della classe KeySequence
KeySequence keys;

// Pin per il pulsante di trigger (esempio con GPIO5)
const int buttonPin = 5;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  // Inizializza la tastiera USB
  keys.begin();
  
  // Abilita il debug seriale
  keys.setDebug(true);
  
  // Configura il pulsante con pull-up interno
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Imposta un delay di default di 100ms tra le sequenze
  keys.setDefaultDelay(100);
  
  // Disabilita l'auto-release per alcune macro
  keys.setAutoRelease(false);
}

void loop() {
  // Gestione debounce del pulsante
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) { // Pulsante premuto
      // Esempio 1: Apertura del blocco note in Windows
      keys.setAutoRelease(true); // Riabilita l'auto-release
      keys.sendSequence("{GUI}r{DELAY200}notepad{ENTER}");
      delay(1000); // Attendi l'apertura del blocco note
      
      // Esempio 2: Scrittura di testo con formattazione
      keys.sendSequence("ESP32-S3 Test{ENTER}");
      keys.sendSequence("{CTRL}b"); // Bold
      keys.sendSequence("Questo testo e' in grassetto{ENTER}");
      keys.sendSequence("{CTRL}b"); // Disattiva Bold
      
      // Esempio 3: Combinazione di tasti senza auto-release
      keys.setAutoRelease(false);
      keys.sendSequence("{CTRL}{SHIFT}"); // Mantiene CTRL+SHIFT premuti
      keys.sendSequence("test"); // Scrive in maiuscolo a causa di SHIFT
      keys.sendSequence("{RELEASE}"); // Rilascia esplicitamente
      
      // Esempio 4: Uso dei ritardi inline
      keys.sendSequence("{DELAY500}"); // Attende 500ms
      keys.sendSequence("Testo dopo il ritardo{ENTER}");
      
      // Esempio 5: Salvataggio del file
      keys.setAutoRelease(true);
      keys.sendSequence("{CTRL}s"); // CTRL+S
      delay(500);
      keys.sendSequence("test_esp32s3.txt{ENTER}");
    }
  }
  
  lastButtonState = reading;
}
