# Biblioteca Virtuale - Versione Finale con Visitor Pattern

Un progetto di Programmazione ad Oggetti in C++ con Qt per la gestione di una biblioteca virtuale che utilizza il pattern Visitor per la visualizzazione dei media nell'interfaccia grafica.

## Caratteristiche Principali

### Architettura
- **Separazione Logica/Grafica**: Modello logico completamente indipendente dall'interfaccia utente
- **Pattern Visitor per GUI**: Implementazione del pattern Visitor per generare widget specifici per ogni tipo di media
- **Polimorfismo**: Gerarchia di classi con Media (base astratta), Book, Film, MagazineArticle
- **Persistenza JSON**: Salvataggio e caricamento dati con serializzazione/deserializzazione

### Funzionalità Complete
- **Sistema di Login**: Autenticazione utente con username/password (default: admin/admin)
- **Gestione Immagini**: Supporto per immagini di copertina con anteprima
- **Operazioni CRUD**: Aggiunta, modifica ed eliminazione di media tramite dialog dedicati
- **Interfaccia a Schede**: Organizzazione per tipo di media con visualizzazione a griglia
- **Ricerca**: Funzionalità di ricerca per titolo
- **Visualizzazione Visitor**: Widget personalizzati per ogni tipo di media

### Tipi di Media Supportati
1. **Libri**: Titolo, Anno, Autore, ISBN, Editore, Copertina
2. **Film**: Titolo, Anno, Regista, Durata, Genere, Poster
3. **Articoli di Rivista**: Titolo, Anno, Autore, Rivista, DOI, Copertina

## Struttura del Progetto

```
biblioteca_virtuale/
├── model/                      # Modello logico
│   ├── Media.h/.cpp           # Classe base astratta
│   ├── Book.h/.cpp            # Classe per libri
│   ├── Film.h/.cpp            # Classe per film
│   ├── MagazineArticle.h/.cpp # Classe per articoli
│   ├── MediaVisitor.h         # Interfaccia Visitor
│   ├── Biblioteca.h/.cpp      # Gestione collezione
│   ├── User.h/.cpp            # Gestione utenti
│   └── UserAuthenticator.h/.cpp # Autenticazione
├── view/                       # Interfaccia grafica
│   ├── MainWindow.h/.cpp      # Finestra principale con griglia
│   ├── LoginDialog.h/.cpp     # Finestra di login
│   ├── AddMediaDialog.h/.cpp  # Dialog aggiunta media
│   ├── EditMediaDialog.h/.cpp # Dialog modifica media
│   └── MediaWidgetVisitor.h/.cpp # Visitor per widget GUI
├── persistence/               # Persistenza dati
│   └── JsonSerializer.h/.cpp  # Serializzazione JSON
├── resources/                 # Risorse
│   └── images/               # Immagini di copertina
├── data/                     # File dati
│   ├── biblioteca_esempio.json # Dati di esempio
│   └── users.json           # Database utenti
└── tests/                   # Test unitari
```

## Pattern Visitor Implementato

### Utilizzo del Pattern per GUI
Il pattern Visitor è utilizzato per generare widget specifici per ogni tipo di media:

```cpp
class MediaVisitor {
public:
    virtual ~MediaVisitor() = default;
    virtual QWidget* visit(Book* book) = 0;
    virtual QWidget* visit(Film* film) = 0;
    virtual QWidget* visit(MagazineArticle* article) = 0;
};
```

### MediaWidgetVisitor
Implementazione concreta che genera widget personalizzati:

```cpp
class MediaWidgetVisitor : public MediaVisitor {
public:
    QWidget* visit(Book* book) override {
        // Crea widget specifico per libri con icona 📚
        QWidget* widget = createBaseWidget(book->getTitle(), book->getYear(), book->getCoverImagePath());
        addBookSpecificInfo(widget, book->getAuthor(), book->getIsbn(), book->getPublisher());
        return widget;
    }
    
    QWidget* visit(Film* film) override {
        // Crea widget specifico per film con icona 🎬
        QWidget* widget = createBaseWidget(film->getTitle(), film->getYear(), film->getCoverImagePath());
        addFilmSpecificInfo(widget, film->getDirector(), film->getDuration(), film->getGenre());
        return widget;
    }
    
    QWidget* visit(MagazineArticle* article) override {
        // Crea widget specifico per articoli con icona 📄
        QWidget* widget = createBaseWidget(article->getTitle(), article->getYear(), article->getCoverImagePath());
        addMagazineArticleSpecificInfo(widget, article->getAuthor(), article->getMagazine(), article->getDoi());
        return widget;
    }
};
```

### Utilizzo nell'Interfaccia
```cpp
// In MainWindow, per ogni media:
MediaWidgetVisitor* visitor = new MediaWidgetVisitor();
QWidget* mediaWidget = media->accept(*visitor);
// Il widget viene aggiunto alla griglia dell'interfaccia
```

## Caratteristiche dell'Interfaccia

### Layout a Griglia
- **Visualizzazione a Schede**: Una scheda per ogni tipo di media
- **Griglia Responsive**: 3 colonne per riga, scroll automatico
- **Widget Personalizzati**: Ogni media ha un widget specifico generato dal Visitor
- **Selezione Visuale**: Click sui widget per selezionare i media

### Widget Media
Ogni widget generato dal Visitor include:
- **Indicatore Tipo**: Badge colorato con icona (📚 Libro, 🎬 Film, 📄 Articolo)
- **Anteprima Copertina**: Immagine ridimensionata automaticamente
- **Informazioni Base**: Titolo, anno, informazioni specifiche per tipo
- **Stile Coerente**: Design uniforme con bordi arrotondati e ombre

### Interazione
- **Click per Selezione**: Click sui widget per selezionare i media
- **Feedback Visuale**: Bordo blu per indicare la selezione
- **Operazioni CRUD**: Pulsanti per aggiungere, modificare, eliminare
- **Ricerca**: Barra di ricerca per ogni tipo di media

## Compilazione e Esecuzione

### Prerequisiti
- Qt5 (qtbase5-dev, qt5-qmake)
- Compilatore C++ con supporto C++17
- Make

### Compilazione
```bash
cd biblioteca_virtuale
qmake
make
```

### Esecuzione
```bash
./biblioteca_virtuale
```

## Utilizzo

### Login
- Username: `admin`
- Password: `admin`

### Operazioni Principali
1. **Visualizzazione**: I media sono mostrati come widget in griglia
2. **Selezione**: Click sui widget per selezionare un media
3. **Aggiunta**: Pulsante "Aggiungi" in ogni scheda
4. **Modifica**: Selezionare un media e cliccare "Modifica"
5. **Eliminazione**: Selezionare un media e cliccare "Elimina"
6. **Ricerca**: Utilizzare la barra di ricerca in ogni scheda
7. **Salvataggio/Caricamento**: Menu File per gestire la persistenza

### Gestione Immagini
- Le immagini di copertina sono selezionabili dalla cartella `resources/images/`
- Anteprima automatica nei widget e nei dialog
- Supporto per formati PNG, JPG, JPEG, GIF

## Vantaggi del Pattern Visitor

### Per la GUI
- **Widget Specifici**: Ogni tipo di media ha un widget personalizzato
- **Estensibilità**: Facile aggiunta di nuovi tipi di visualizzazione
- **Separazione delle Responsabilità**: Logica di visualizzazione separata dal modello
- **Riutilizzabilità**: I visitor possono essere utilizzati in contesti diversi

### Esempi di Estensione
```cpp
// Visitor per esportazione PDF
class MediaPDFExportVisitor : public MediaVisitor {
    QWidget* visit(Book* book) override {
        // Genera layout per esportazione PDF di libri
    }
};

// Visitor per statistiche
class MediaStatsVisitor : public MediaVisitor {
    QWidget* visit(Book* book) override {
        // Genera widget con statistiche per libri
    }
};
```

## Requisiti Soddisfatti

✅ **Gerarchia di Classi**: Media (base astratta) con Book, Film, MagazineArticle  
✅ **Polimorfismo Non Banale**: Metodi virtuali e factory pattern  
✅ **Pattern Visitor**: Implementato per generazione widget GUI  
✅ **Interfaccia Grafica Qt**: Layout a griglia con widget personalizzati  
✅ **Persistenza JSON**: Serializzazione/deserializzazione completa  
✅ **Separazione Architetturale**: Modello indipendente dalla GUI  
✅ **Sistema di Login**: Autenticazione con hash password  
✅ **Gestione Immagini**: Selezione e anteprima copertine  
✅ **Operazioni CRUD**: Aggiunta, modifica, eliminazione complete  
✅ **Utilizzo Visitor in GUI**: Widget generati tramite pattern Visitor  

## Differenze dalle Versioni Precedenti

### Evoluzione del Pattern Visitor
1. **Prima Versione**: Visitor per operazioni CRUD (uso improprio)
2. **Seconda Versione**: Visitor per tipi di media (uso corretto ma limitato)
3. **Versione Finale**: Visitor per generazione widget GUI (uso avanzato e pratico)

### Miglioramenti dell'Interfaccia
- **Da Lista a Griglia**: Visualizzazione più ricca e intuitiva
- **Widget Personalizzati**: Ogni media ha un aspetto specifico
- **Interazione Migliorata**: Selezione visuale e feedback immediato
- **Design Moderno**: Stile coerente con icone e colori

## Autore

Progetto sviluppato per il corso di Programmazione ad Oggetti.

## Note Tecniche

- Utilizzo di Qt5 per l'interfaccia grafica
- Pattern Visitor utilizzato per generazione widget GUI
- Serializzazione JSON nativa di Qt
- Hash SHA-256 per le password
- Gestione memoria con RAII e smart pointers
- Event filtering per gestione click sui widget
- Layout responsive con QGridLayout e QScrollArea
- Rispetto delle convenzioni di naming Qt/C++

