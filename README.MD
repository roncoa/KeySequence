# Documentazione Libreria KeySequence

## Panoramica
KeySequence è una libreria Arduino per l'invio di sequenze complesse di tasti, inclusi tasti speciali, ritardi e modificatori. Supporta tutti i tasti standard della tastiera, i tasti funzione e i tasti modificatori con controllo avanzato sulla temporizzazione della pressione e del rilascio dei tasti.

## Caratteristiche
- Supporto per tutti i tasti standard della tastiera
- Tasti speciali e modificatori (CTRL, ALT, SHIFT, ecc.)
- Varianti sinistra/destra dei tasti modificatori
- Ritardi personalizzabili
- Controllo esplicito del rilascio tasti
- Gestione del buffer per pressioni simultanee di tasti
- Validazione dell'input
- Modalità debug con output seriale
- Messaggi di errore efficienti salvati in PROGMEM

## Installazione
1. Creare una cartella chiamata "KeySequence" nella directory delle librerie Arduino
2. Copiare KeySequence.h, KeySequence.cpp e keywords.txt in questa cartella
3. Riavviare l'IDE Arduino
4. Includere la libreria nel proprio sketch con `#include "KeySequence.h"`

## Requisiti Hardware
- Scheda Arduino con supporto tastiera USB (es. Leonardo, Micro, Pro Micro)
- Connessione USB al computer host

## Utilizzo Base

### Inizializzazione
```cpp
#include "KeySequence.h"

KeySequence keys;

void setup() {
  keys.begin();
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
static const unsigned int KEY_PRESS_DELAY = 5;         // ms tra pressione
static const unsigned int KEY_RELEASE_DELAY = 5;       // ms tra tasti
static const int BUFFER_SIZE = 10;                     // max tasti simultanei
static const unsigned int MAX_SEQUENCE_LENGTH = 256;   // max caratteri sequenza
static const unsigned int MAX_SPECIAL_KEY_LENGTH = 20; // max lunghezza tasto speciale
static const unsigned int MAX_DELAY_VALUE = 10000;     // max ritardo ms
```

### Utilizzo Memoria
- Messaggi di errore salvati in PROGMEM
- Buffer dinamico per i tasti premuti
- Validazione statica delle stringhe
- Impronta RAM minima

### Temporizzazione
- 5ms minimo tra pressione/rilascio tasti
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
   // Fallirà la validazione se > 256 caratteri
   String sequenzaLunga = "...";
   ```

2. **Buffer Overflow**
   ```cpp
   // Troppi tasti simultanei
   keys.sendSequence("{CTRL}{ALT}{SHIFT}{WIN}{F1}{F2}{F3}{F4}{F5}{F6}{F7}");
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

## Consigli e Best Practices

### Prestazioni
1. Mantenere le sequenze brevi e mirate
2. Usare ritardi ragionevoli (100-500ms)
3. Rilasciare i tasti quando si cambia contesto
4. Validare le sequenze prima dell'invio
5. Attenzione alla dimensione del buffer per combinazioni complesse

### Affidabilità
1. Testare accuratamente le sequenze
2. Usare la modalità debug durante lo sviluppo
3. Gestire gli errori appropriatamente
4. Considerare la latenza del sistema host
5. Aggiungere ritardi per applicazioni più lente

### Manutenibilità
1. Documentare le sequenze
2. Usare nomi variabili significativi
3. Raggruppare sequenze correlate
4. Commentare combinazioni complesse
5. Seguire formattazione consistente

## Risoluzione Problemi

### Problemi Comuni

1. **Tasti Non Registrati**
   - Controllare connessione USB
   - Verificare compatibilità scheda
   - Ridurre velocità sequenza
   - Controllare overflow buffer

2. **Comportamento Inconsistente**
   - Aggiungere più ritardi
   - Controllare punti di rilascio tasti
   - Verificare stato sistema host
   - Testare con modalità debug

3. **Caratteri Errati**
   - Controllare layout tastiera
   - Verificare nomi tasti speciali
   - Controllare maiuscole/minuscole
   - Testare senza modificatori

### Passi Debug
1. Abilitare modalità debug
2. Testare sequenze individuali
3. Monitorare output seriale
4. Validare prima le sequenze
5. Controllare problemi di timing

## Struttura Progetto
```
KeySequence/
│
├── KeySequence.h       // File header principale
├── KeySequence.cpp     // File implementazione
├── keywords.txt        // Keywords IDE
└── examples/
    └── EsempioBase/   // Sketch di esempio
```

## Cronologia Versioni

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

## Crediti
Sviluppata da [Il Tuo Nome]
Basata su Arduino Keyboard Library

## Supporto
Per supporto, si prega di:
1. Controllare la documentazione
2. Abilitare modalità debug
3. Cercare issues esistenti
4. Creare una nuova issue se necessario
5. Contattare i maintainer

## FAQ

### Domande Generali

**D: Quali schede Arduino sono supportate?**
R: Qualsiasi scheda con supporto USB nativo (Leonardo, Micro, Pro Micro, ecc.)

**D: Qual è la lunghezza massima della sequenza?**
R: 256 caratteri

**D: Quanti tasti possono essere premuti simultaneamente?**
R: 10 tasti (BUFFER_SIZE)

### Domande Tecniche

**D: Perché le mie combinazioni di tasti non funzionano?**
R: Controllare:
1. Punti di rilascio tasti
2. Limiti dimensione buffer
3. Timing/ritardi
4. Supporto sistema host

**D: Come aggiungo tasti personalizzati?**
R: Modificare il metodo processSpecialKey in KeySequence.cpp

**D: Posso usarla per macro di gioco?**
R: Sì, ma fare attenzione a:
1. Politiche anti-cheat
2. Limitazioni timing
3. Requisiti gioco
