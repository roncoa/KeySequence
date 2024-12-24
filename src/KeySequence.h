// KeySequence.h
#ifndef KeySequence_h
#define KeySequence_h

#include "Arduino.h"
#include "Keyboard.h"

class KeySequence {
 public:
   // Costruttore e metodi pubblici
   KeySequence();
   void begin();                                        // Inizializza la tastiera
   void sendSequence(String sequence);                  // Invia sequenza con delay predefinito
   void sendSequenceWithDelay(String sequence, int delayTime); // Invia sequenza con delay specificato
   void setDefaultDelay(int delay);                    // Imposta delay predefinito
   int getDefaultDelay();                              // Ottiene delay predefinito
   void setDebug(bool enabled);                        // Abilita/disabilita debug
   bool isDebugEnabled();                              // Stato debug
   bool validateSequence(String sequence);              // Valida una sequenza
   
 private:
   // Metodi privati
   bool processSpecialKey(String specialKey);          // Processa un tasto speciale
   void pressKey(char key);                            // Preme un tasto senza rilasciarlo
   
   // Costanti per i limiti
   static const unsigned int KEY_PRESS_DELAY = 5;         // ms tra pressioni
   static const unsigned int KEY_RELEASE_DELAY = 5;       // ms tra tasti
   static const int BUFFER_SIZE = 10;                     // max tasti contemporanei
   static const unsigned int MAX_SEQUENCE_LENGTH = 256;   // lunghezza max sequenza
   static const unsigned int MAX_SPECIAL_KEY_LENGTH = 20; // lunghezza max comando speciale
   static const unsigned int MAX_DELAY_VALUE = 10000;     // valore max delay (10s)

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
   byte pressedKeys[BUFFER_SIZE];  // Buffer tasti premuti
   int pressedCount;               // Numero tasti premuti
   int defaultDelay;               // Delay predefinito
   bool debug;                     // Stato debug
};

#endif
