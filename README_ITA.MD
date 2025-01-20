# Documentazione Libreria KeySequence

## Panoramica
KeySequence è una libreria Arduino/ESP32 per l'invio di sequenze complesse di tasti, inclusi tasti speciali, ritardi e modificatori. Supporta tutti i tasti standard della tastiera, i tasti funzione e i tasti modificatori con controllo avanzato sulla temporizzazione della pressione e del rilascio dei tasti.

## Caratteristiche
- Supporto per tutte le piattaforme USB HID (Arduino e ESP32)
- Tasti standard della tastiera, speciali e modificatori (CTRL, ALT, SHIFT, ecc.)
- Varianti sinistra/destra dei tasti modificatori
- Ritardi personalizzabili
- Controllo esplicito del rilascio tasti
- Gestione del buffer per pressioni simultanee di tasti
- Validazione dell'input
- Modalità debug con output seriale
- Messaggi di errore efficienti salvati in PROGMEM
- Gestione automatica dell'inizializzazione USB per tutte le piattaforme

## Installazione
1. Creare una cartella chiamata "KeySequence" nella directory delle librerie Arduino
2. Copiare KeySequence.h, KeySequence.cpp e keywords.txt in questa cartella
3. Riavviare l'IDE Arduino
4. Includere la libreria nel proprio sketch con `#include "KeySequence.h"`

## Requisiti Hardware
- Scheda Arduino con supporto tastiera USB (es. Leonardo, Micro, Pro Micro)
- ESP32 con supporto USB (es. ESP32-S3)
- Connessione USB al computer host

## Utilizzo Base

### Inizializzazione
```cpp
#include "KeySequence.h"

KeySequence keys;

void setup() {
  keys.begin();  // Gestisce automaticamente l'inizializzazione per Arduino e ESP32
}
```

### Sequenze Semplici
```cpp
// Invia testo
keys.sendSequence("Ciao Mondo");

// Invia tasti speciali
keys.sendSequence("{ENTER}");
keys.sendSequence("{TAB}");

// Combinazioni
keys.sendSequence("{CTRL}c");  // CTRL+C
```

## Comandi Speciali

### Tasti Modificatori
- `{ALT}`, `{RALT}`
- `{CTRL}`, `{RCTRL}`
- `{SHIFT}`, `{RSHIFT}`
- `{GUI}` o `{WIN}`, `{RGUI}` o `{RWIN}`

### Tasti di Navigazione
- `{UP}`, `{DOWN}`, `{LEFT}`, `{RIGHT}`
- `{HOME}`, `{END}`
- `{PAGEUP}` o `{PGUP}`
- `{PAGEDOWN}` o `{PGDN}`

### Tasti di Modifica
- `{ENTER}` o `{RETURN}`
- `{TAB}`
- `{BACKSPACE}` o `{BKSP}`
- `{DELETE}` o `{DEL}`
- `{INSERT}` o `{INS}`
- `{ESC}`

### Tasti Funzione
- Da `{F1}` a `{F12}`

### Tasti Speciali
- `{SPACE}`
- `{PRINTSCREEN}` o `{PRTSC}`
- `{SCROLLLOCK}` o `{SCRLK}`
- `{PAUSE}` o `{BREAK}`
- `{NUMLOCK}` o `{NUMLK}`
- `{CAPSLOCK}` o `{CAPS}`
- `{MENU}` o `{APP}`

### Comandi di Controllo
- `{DELAY}` o `{DELAY0}`: Nessun ritardo
- `{DELAY500}`: Attesa di 500ms
- `{RELEASE}`: Rilascia tutti i tasti premuti

## Funzionalità Avanzate

### Comportamento del Rilascio Tasti
Di default, i tasti rimangono premuti fino a uno di questi eventi:
- Fine della sequenza
- Comando esplicito `{RELEASE}`
- Inizio di una nuova sequenza

Questo permette combinazioni complesse come:
```cpp
// CTRL rimane premuto per entrambi c e v
keys.sendSequence("{CTRL}cv");

// CTRL è premuto solo per c
keys.sendSequence("{CTRL}c{RELEASE}v");
```

### Controllo Auto-Release
È possibile controllare se i tasti vengono rilasciati automaticamente alla fine di ogni sequenza:
```cpp
// Disabilita il rilascio automatico alla fine della sequenza
keys.setAutoRelease(false);

// Riabilita il rilascio automatico (default)
keys.setAutoRelease(true);

// Verifica lo stato corrente
bool isAutoRelease = keys.isAutoReleaseEnabled();
```

Di default autoRelease è abilitato (true) per mantenere la compatibilità con le versioni precedenti.

### Ritardi
```cpp
// Imposta il ritardo predefinito tra le sequenze
keys.setDefaultDelay(200);

// Usa ritardi inline
keys.sendSequence("Primo{DELAY500}Secondo");

// Invia con ritardo specifico
keys.sendSequenceWithDelay("Test", 1000);
```

### Modalità Debug
La modalità debug fornisce feedback dettagliato attraverso l'interfaccia Seriale:
```cpp
// Abilita i messaggi di debug (inizializza anche Serial)
keys.setDebug(true);

// Controlla se il debug è abilitato
bool isDebug = keys.isDebugEnabled();
```

I messaggi di debug includono:
- Errori di validazione
- Tasti non riconosciuti
- Avvisi di buffer overflow
- Errori di parsing della sequenza

### Validazione Sequenza
È possibile validare le sequenze prima di inviarle:
```cpp
if(keys.validateSequence("{CTRL}c")) {
  // La sequenza è valida
  keys.sendSequence("{CTRL}c");
}
```

Il validatore controlla:
- Limiti lunghezza sequenza
- Parentesi graffe bilanciate
- Tasti speciali validi
- Valori di ritardo validi

## Specifiche Tecniche

### Costanti
```cpp
static const unsigned int KEY_PRESS_DELAY = 20;        // ms tra pressione
static const int BUFFER_SIZE = 5;                      // max tasti simultanei
static const unsigned int MAX_SEQUENCE_LENGTH = 128;   // max caratteri sequenza
static const unsigned int MAX_SPECIAL_KEY_LENGTH = 10; // max lunghezza tasto speciale
static const unsigned int MAX_DELAY_VALUE = 10000;     // max ritardo ms
```

### Utilizzo Memoria
- Messaggi di errore salvati in PROGMEM
- Buffer dinamico per i tasti premuti
- Validazione statica delle stringhe
- Impronta RAM minima

### Temporizzazione
- 20ms minimo tra pressione/rilascio tasti (ottimizzato per ESP32)
- 100ms predefiniti tra sequenze
- Ritardi personalizzabili fino a 10 secondi
- Timing preciso usando `delay()` di Arduino

## Esempi di Utilizzo

### Comandi Editor di Testo
```cpp
// Seleziona tutto
keys.sendSequence("{CTRL}a");

// Copia-incolla con rilascio esplicito
keys.sendSequence("{CTRL}c{RELEASE}{CTRL}v");

// Salva file
keys.sendSequence("{CTRL}s");
```

### Gestione Finestre
```cpp
// Alt-Tab cambio finestra
keys.sendSequence("{ALT}{TAB}");

// Apri dialogo Esegui
keys.sendSequence("{GUI}r");

// Task Manager
keys.sendSequence("{CTRL}{SHIFT}{ESC}");
```

### Macro per Giochi
```cpp
// Premi più tasti
keys.sendSequence("{SHIFT}{W}");

// Azioni rapide con ritardi
keys.sendSequence("1{DELAY200}2{DELAY200}3");

// Combinazioni complesse
keys.sendSequence("{CTRL}{SPACE}{DELAY100}e");
```

## Gestione Errori

### Errori Comuni
1. **Sequenza Troppo Lunga**
   ```cpp
   // Fallirà la validazione se > 128 caratteri
   String sequenzaLunga = "...";
   ```

2. **Buffer Overflow**
   ```cpp
   // Troppi tasti simultanei (max 5)
   keys.sendSequence("{CTRL}{ALT}{SHIFT}{WIN}{F1}{F2}");
   ```

3. **Parentesi non Valide**
   ```cpp
   // Parentesi chiusa mancante
   keys.sendSequence("{CTRL");
   
   // Parentesi aperta mancante
   keys.sendSequence("CTRL}");
   ```

### Esempi Output Debug
```
Errore: sequenza troppo lunga
Errore: parentesi graffe non bilanciate
Errore: buffer tasti pieno
Errore: valore delay non numerico
Tasto speciale non riconosciuto: XYZ
```

## Note Specifiche per Piattaforma

### Arduino Leonardo/Micro
- Compatibilità nativa con la libreria Keyboard di Arduino
- Timing ottimale per la maggior parte delle applicazioni
- Supporto completo per tutte le funzionalità

### ESP32 (S3)
- Supporto USB HID nativo
- Inizializzazione USB automatica
- Timing ottimizzato (20ms KEY_PRESS_DELAY)
- Stessa API di Arduino per massima compatibilità

## Struttura Progetto
```
KeySequence/
│
├── examples/
│   └── Basic/
│       └── Basic.ino          // Esempio funzionante su tutte le piattaforme
│
├── src/
│   ├── KeySequence.h
│   └── KeySequence.cpp
│
├── keywords.txt
└── library.properties
```

## Cronologia Versioni

### v1.2.1 (Versione Corrente)
- BugFix pressKey
- Esegue releaseAll() prima di ENTER

### v1.2.0
- Aggiunto supporto per ESP32 (testato su ESP32-S3)
- Gestione automatica dell'inizializzazione USB per ESP32
- Aumentato KEY_PRESS_DELAY default a 20ms per migliore compatibilità
- Unified codebase per Arduino e ESP32
- Ottimizzazione delle costanti di memoria
- Ridotto BUFFER_SIZE a 5 per ottimizzare l'uso della RAM
- Ridotto MAX_SEQUENCE_LENGTH a 128 caratteri
- Ridotto MAX_SPECIAL_KEY_LENGTH a 10 caratteri

### v1.1.1
- Bug fix, migliorie varie

### v1.1.0
- Aggiunto controllo configurabile dell'auto-release dei tasti
- Nuovi metodi setAutoRelease() e isAutoReleaseEnabled()
- Comportamento retrocompatibile (auto-release abilitato di default)

### v1.0.0
- Release iniziale
- Supporto sequenze base
- Gestione tasti speciali
- Controllo ritardi
- Funzionalità debug

### Funzionalità Pianificate
- Layout tastiera multipli
- Registrazione macro
- Concatenamento sequenze
- Definizioni tasti personalizzate
- Gestione errori migliorata

## Come Contribuire
1. Fork del repository
2. Crea il tuo branch feature
3. Committa i tuoi cambiamenti
4. Pusha sul branch
5. Crea una Pull Request

## Licenza
Questa libreria è rilasciata sotto licenza MIT. Vedi file LICENSE per i dettagli.

## Supporto
Per supporto, si prega di:
1. Controllare la documentazione
2. Abilitare modalità debug
3. Cercare issues esistenti
4. Creare una nuova issue se necessario
5. Contattare i maintainer

## FAQ

### Domande Generali

**D: Quali schede sono supportate?**
R: Tutte le schede con supporto USB HID nativo:
- Arduino (Leonardo, Micro, Pro Micro)
- ESP32 con supporto USB (come ESP32-S3)

**D: Qual è la lunghezza massima della sequenza?**
R: 128 caratteri

**D: Quanti tasti possono essere premuti simultaneamente?**
R: 5 tasti (BUFFER_SIZE)

### Domande Tecniche

**D: Perché le mie combinazioni di tasti non funzionano?**
R: Controllare:
1. Punti di rilascio tasti
2. Limiti dimensione buffer (max 5 tasti)
3. Timing/ritardi (20ms minimo)
4. Supporto sistema host

**D: Come aggiungo tasti personalizzati?**
R: Modificare il metodo processSpecialKey in KeySequence.cpp

**D: Ci sono differenze tra Arduino e ESP32?**
R: No, la libreria gestisce automaticamente le differenze tra le piattaforme. Lo stesso codice funziona su entrambe senza modifiche.
