/*
  KeySequence - Basic Example
  Questo esempio mostra le funzionalità base della libreria KeySequence.
  Funziona sia su Arduino Leonardo che su ESP32-S3.
*/

#include "KeySequence.h"

KeySequence keySeq;  // Crea un'istanza della classe KeySequence

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);   // Attende che la porta seriale sia pronta
  Serial.println("Test KeySequence - Basic Example");
  
  keySeq.begin();             // Inizializza la tastiera (gestisce automaticamente USB per ESP32)
  keySeq.setDebug(true);      // Abilita i messaggi di debug
  keySeq.setDefaultDelay(100);// Imposta il ritardo predefinito tra le sequenze
  
  // Attende qualche secondo per stabilizzare la connessione USB
  delay(3000);
  Serial.println("Iniziamo i test:");
}

void loop() {
  // Test 1: Testo semplice
  Serial.println("\n=== Test 1: Testo semplice ===");
  keySeq.sendSequence("Hello World");
  delay(1000);
  
  // Test 2: Tasti speciali
  Serial.println("\n=== Test 2: Tasti speciali ===");
  keySeq.sendSequence("{ENTER}");
  delay(1000);
  
  // Test 3: Combinazioni di tasti
  Serial.println("\n=== Test 3: Combinazioni di tasti ===");
  keySeq.sendSequence("{CTRL}c");  // CTRL+C
  delay(1000);
  
  // Test 4: Sequenze con delay
  Serial.println("\n=== Test 4: Sequenze con delay ===");
  keySeq.sendSequence("First{DELAY500}Second");
  delay(1000);

  // Test 5: Combinazioni complesse
  Serial.println("\n=== Test 5: Combinazioni complesse ===");
  keySeq.sendSequence("{CTRL}a{DELAY100}{CTRL}c");  // Seleziona tutto e copia
  delay(1000);

  // Test 6: Tasti di navigazione
  Serial.println("\n=== Test 6: Tasti di navigazione ===");
  keySeq.sendSequence("{UP}{DELAY100}{DOWN}{DELAY100}{LEFT}{DELAY100}{RIGHT}");
  delay(1000);

  // Test 7: Tasti funzione
  Serial.println("\n=== Test 7: Tasti funzione ===");
  keySeq.sendSequence("{F1}");
  delay(1000);

  // Test 8: Release esplicito
  Serial.println("\n=== Test 8: Release esplicito ===");
  keySeq.sendSequence("{CTRL}c{RELEASE}v");  // CTRL premuto solo per C
  delay(1000);

  Serial.println("\n=== Test completato, attendo 10 secondi prima di ricominciare... ===");
  delay(10000);  // Pausa di 10 secondi prima di ripetere i test
}
