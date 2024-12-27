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
  // Inizializza la tastiera
  keys.begin();
  
  // Configurazione LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Configurazione pulsanti con pull-up interno
  pinMode(BTN_MACRO, INPUT_PULLUP);
  pinMode(BTN_WIN, INPUT_PULLUP);
  pinMode(BTN_COMBO, INPUT_PULLUP);
  
  // Abilita debug seriale
  keys.setDebug(true);
  
  // Configura delay predefinito
  keys.setDefaultDelay(50);
}

void loop() {
  // Leggi stato pulsanti
  int btnMacro = digitalRead(BTN_MACRO);
  int btnWin = digitalRead(BTN_WIN);
  int btnCombo = digitalRead(BTN_COMBO);
  
  // Controlla se qualche pulsante è stato premuto
  if (btnMacro == LOW && lastButtonStates[0] == HIGH) {
    blinkLed();
    macroSequence();
  }
  else if (btnWin == LOW && lastButtonStates[1] == HIGH) {
    blinkLed();
    windowsSequence();
  }
  else if (btnCombo == LOW && lastButtonStates[2] == HIGH) {
    blinkLed();
    comboSequence();
  }
  
  // Aggiorna stati precedenti
  lastButtonStates[0] = btnMacro;
  lastButtonStates[1] = btnWin;
  lastButtonStates[2] = btnCombo;
  
  // Piccolo delay per stabilità
  delay(10);
}

// Sequenza 1: Macro di testo
void macroSequence() {
  keys.setAutoRelease(true);
  
  // Apre il blocco note
  keys.sendSequence("{GUI}r{DELAY200}notepad{ENTER}");
  delay(1000);
  
  // Scrive un testo formattato
  keys.sendSequence("Test Arduino Leonardo{ENTER}");
  keys.sendSequence("Timestamp: ");
  
  // Simula la data corrente
  keys.sendSequence("2024-12-27");
  keys.sendSequence("{ENTER}{ENTER}");
  
  // Aggiunge del testo formattato
  keys.sendSequence("{CTRL}b");  // Bold on
  keys.sendSequence("Questo testo e' in grassetto");
  keys.sendSequence("{CTRL}b");  // Bold off
  keys.sendSequence("{ENTER}");
  
  // Salva il file
  keys.sendSequence("{CTRL}s");
  delay(500);
  keys.sendSequence("test_leonardo.txt{ENTER}");
}

// Sequenza 2: Scorciatoie Windows
void windowsSequence() {
  keys.setAutoRelease(true);
  
  // Apre Esegui
  keys.sendSequence("{GUI}r");
  delay(200);
  
  // Apre calc
  keys.sendSequence("calc{ENTER}");
  delay(1000);
  
  // Esegue alcuni calcoli
  keys.sendSequence("100");
  keys.sendSequence("{ADD}");  // +
  keys.sendSequence("50");
  keys.sendSequence("{ENTER}");
  
  // Minimizza
  keys.sendSequence("{GUI}{DOWN}");
}

// Sequenza 3: Combinazioni di tasti
void comboSequence() {
  // Esempio di uso dell'autoRelease
  keys.setAutoRelease(false);
  
  // Pressione multipla di tasti
  keys.sendSequence("{CTRL}{ALT}");
  delay(200);
  keys.sendSequence("123");
  keys.sendSequence("{RELEASE}");
  
  delay(500);
  
  // Altra combinazione
  keys.setAutoRelease(true);
  keys.sendSequence("{GUI}{SHIFT}m");  // Win+Shift+M (ripristina finestre)
}

// Feedback visivo
void blinkLed() {
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
}
