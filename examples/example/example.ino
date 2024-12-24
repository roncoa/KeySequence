// example.ino
#include "KeySequence.h"

KeySequence keys;

void setup() {
  keys.setDebug(true);
  keys.begin();
  keys.setDefaultDelay(150);
  delay(2000);
}

void loop() {
  // Test senza release automatico
  keys.sendSequence("{CTRL}cv");  // CTRL rimane premuto per entrambe le lettere
  delay(2000);
  
  // Test con release esplicito
  keys.sendSequence("{CTRL}c{RELEASE}v");  // CTRL solo per la c
  delay(2000);
  
  // Test combinazioni multiple senza release
  keys.sendSequence("{ALT}{TAB}abc");  // ALT rimane premuto
  delay(2000);
  
  // Test sequenza complessa con release
  keys.sendSequence("{CTRL}{ALT}x{RELEASE}yz");
  delay(5000);
}