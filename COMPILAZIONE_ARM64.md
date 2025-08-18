# Istruzioni per Compilazione su ARM64

## Prerequisiti per ARM64

### Ubuntu/Debian ARM64
```bash
sudo apt update
sudo apt install -y build-essential
sudo apt install -y qt5-qmake qtbase5-dev qtbase5-dev-tools
sudo apt install -y libqt5widgets5 libqt5gui5 libqt5core5a
```

### macOS ARM64 (Apple Silicon)
```bash
# Installa Xcode Command Line Tools
xcode-select --install

# Installa Qt tramite Homebrew
brew install qt@5
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
```

### Arch Linux ARM64
```bash
sudo pacman -S base-devel
sudo pacman -S qt5-base qt5-tools
```

## Compilazione

### 1. Pulire eventuali file di compilazione precedenti
```bash
cd biblioteca_virtuale
rm -f *.o moc_* biblioteca_virtuale Makefile .qmake.stash
```

### 2. Generare il Makefile
```bash
qmake biblioteca_virtuale.pro
```

Se `qmake` non è trovato, prova:
```bash
# Ubuntu/Debian
/usr/lib/qt5/bin/qmake biblioteca_virtuale.pro

# macOS con Homebrew
/opt/homebrew/opt/qt@5/bin/qmake biblioteca_virtuale.pro
```

### 3. Compilare
```bash
make
```

### 4. Verificare la compilazione
```bash
ls -la biblioteca_virtuale
file biblioteca_virtuale  # Dovrebbe mostrare "ARM aarch64"
```

## Esecuzione

### Linux ARM64
```bash
./biblioteca_virtuale
```

### macOS ARM64
```bash
./biblioteca_virtuale
```

Se hai problemi con le librerie Qt:
```bash
# Verifica le dipendenze
otool -L biblioteca_virtuale  # macOS
ldd biblioteca_virtuale       # Linux
```

## Risoluzione Problemi Comuni

### Errore: qmake non trovato
```bash
# Trova qmake
find /usr -name qmake 2>/dev/null
find /opt -name qmake 2>/dev/null

# Usa il percorso completo
/percorso/completo/qmake biblioteca_virtuale.pro
```

### Errore: Qt headers non trovati
```bash
# Ubuntu/Debian
sudo apt install qtbase5-dev qtbase5-dev-tools

# Verifica installazione
dpkg -l | grep qt5
```

### Errore: Librerie Qt non trovate a runtime
```bash
# Ubuntu/Debian
sudo apt install libqt5widgets5 libqt5gui5 libqt5core5a

# Verifica librerie
ldconfig -p | grep Qt5
```

### macOS: Errore architettura
Se hai Qt installato per x86_64, disinstalla e reinstalla per ARM64:
```bash
brew uninstall qt@5
arch -arm64 brew install qt@5
```

## Verifica Architettura

### Linux
```bash
file biblioteca_virtuale
# Output atteso: "biblioteca_virtuale: ELF 64-bit LSB executable, ARM aarch64, ..."
```

### macOS
```bash
file biblioteca_virtuale
# Output atteso: "biblioteca_virtuale: Mach-O 64-bit executable arm64"
```

## Note Specifiche per ARM64

1. **Performance**: La compilazione su ARM64 potrebbe essere più lenta rispetto a x86_64
2. **Memoria**: Assicurati di avere almeno 2GB di RAM disponibili durante la compilazione
3. **Qt Version**: Usa Qt 5.12 o superiore per il miglior supporto ARM64
4. **Cross-compilation**: Non è consigliata per questo progetto; compila nativamente su ARM64

## Test Post-Compilazione

1. **Avvio applicazione**: `./biblioteca_virtuale`
2. **Login**: username `admin`, password `admin`
3. **Test CRUD**: Prova ad aggiungere/modificare/eliminare un media
4. **Test persistenza**: Salva e ricarica la biblioteca
5. **Test immagini**: Verifica il caricamento delle copertine

## Struttura File Sorgente

Il progetto include solo i file sorgente (.h, .cpp) e di configurazione (.pro), senza binari compilati:

```
biblioteca_virtuale/
├── model/           # File sorgente del modello
├── view/            # File sorgente dell\interfaccia
├── persistence/     # File sorgente per persistenza
├── resources/       # Immagini di esempio
├── data/           # File JSON di esempio
├── *.pro           # File di progetto Qt
└── README.md       # Documentazione
```

Tutti i file `.o`, `moc_*`, `Makefile` e l\eseguibile sono stati rimossi per garantire la compatibilità cross-platform.

