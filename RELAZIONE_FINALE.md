# Relazione Tecnica Finale - Biblioteca Virtuale con Pattern Visitor

## Introduzione

Questo documento descrive l'implementazione finale del progetto "Biblioteca Virtuale", un'applicazione C++ con Qt che utilizza il pattern Visitor per la generazione di widget specifici per ogni tipo di media nell'interfaccia grafica.

## Obiettivi del Progetto

### Requisiti Funzionali
- Sistema di gestione biblioteca virtuale con operazioni CRUD
- Supporto per tre tipi di media: Libri, Film, Articoli di Rivista
- Interfaccia grafica Qt con separazione logica/GUI
- Sistema di autenticazione utente
- Persistenza dati in formato JSON
- Gestione immagini di copertina

### Requisiti Tecnici
- Utilizzo del pattern Visitor per gestione tipologie di media
- Gerarchia di classi con polimorfismo non banale
- Separazione architetturale tra modello logico e interfaccia
- Implementazione in C++ con Qt5

## Architettura del Sistema

### Pattern Visitor per GUI

#### Problema Risolto
L'interfaccia grafica necessitava di visualizzare ogni tipo di media con widget specifici e personalizzati, mantenendo la separazione tra logica di business e presentazione.

#### Soluzione Implementata
```cpp
class MediaVisitor {
public:
    virtual ~MediaVisitor() = default;
    virtual QWidget* visit(Book* book) = 0;
    virtual QWidget* visit(Film* film) = 0;
    virtual QWidget* visit(MagazineArticle* article) = 0;
};
```

Il pattern Visitor è utilizzato per:
- **Generare widget specifici** per ogni tipo di media
- **Mantenere la separazione** tra modello e vista
- **Permettere estensioni** senza modificare le classi esistenti

### MediaWidgetVisitor

#### Implementazione
```cpp
class MediaWidgetVisitor : public MediaVisitor {
public:
    QWidget* visit(Book* book) override {
        QWidget* widget = createBaseWidget(book->getTitle(), book->getYear(), book->getCoverImagePath());
        addBookSpecificInfo(widget, book->getAuthor(), book->getIsbn(), book->getPublisher());
        return widget;
    }
    // ... implementazioni per Film e MagazineArticle
};
```

#### Caratteristiche dei Widget Generati
- **Widget Base Comune**: Layout, dimensioni, stile di base
- **Informazioni Specifiche**: Campi diversi per ogni tipo di media
- **Indicatori Visivi**: Badge colorati con icone (📚, 🎬, 📄)
- **Gestione Immagini**: Anteprima copertine con fallback

### Gerarchia delle Classi

```
Media (classe base astratta)
├── Book (libri)
├── Film (film)
└── MagazineArticle (articoli di rivista)
```

#### Metodi Virtuali Implementati
- `visualizzaDettagli()`: Rappresentazione testuale
- `serializza()`: Conversione a JSON
- `accept(MediaVisitor&)`: Accettazione del visitor (restituisce QWidget*)

#### Factory Pattern
```cpp
Media* Media::deserializza(const QJsonObject& jsonObject) {
    QString type = jsonObject["type"].toString();
    if (type == "Book") return Book::deserializza(jsonObject);
    if (type == "Film") return Film::deserializza(jsonObject);
    if (type == "MagazineArticle") return MagazineArticle::deserializza(jsonObject);
    return nullptr;
}
```

## Interfaccia Grafica

### Layout Principale
- **QTabWidget**: Organizzazione per tipo di media
- **QScrollArea**: Contenimento con scroll automatico
- **QGridLayout**: Disposizione a griglia (3 colonne)
- **Widget Personalizzati**: Generati tramite MediaWidgetVisitor

### Interazione Utente
```cpp
// Gestione click sui widget
bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (widget && widget->property("clickable").toBool()) {
            Media* media = static_cast<Media*>(widget->property("media").value<void*>());
            selectedMedia = media;
            // Aggiornamento selezione visuale
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
```

### Caratteristiche Visive
- **Selezione Visuale**: Bordo blu per media selezionato
- **Feedback Immediato**: Aggiornamento status bar
- **Design Coerente**: Stile uniforme per tutti i widget
- **Responsive**: Adattamento automatico alle dimensioni

## Operazioni CRUD

### Aggiunta Media
```cpp
void MainWindow::addMedia() {
    AddMediaDialog *addDialog = new AddMediaDialog(biblioteca, this);
    if (addDialog->exec() == QDialog::Accepted) {
        updateMediaDisplay(); // Rigenera i widget tramite Visitor
        statusBar()->showMessage("Media aggiunto con successo", 2000);
    }
}
```

### Visualizzazione con Visitor
```cpp
void MainWindow::addMediaWidgetToGrid(Media* media, int row, int col) {
    QWidget* mediaWidget = media->accept(*widgetVisitor);
    if (mediaWidget) {
        mediaWidget->setProperty("media", QVariant::fromValue(static_cast<void*>(media)));
        mediaWidget->installEventFilter(this);
        gridLayout->addWidget(mediaWidget, row, col, Qt::AlignTop);
    }
}
```

## Persistenza Dati

### Serializzazione JSON
Ogni classe implementa la serializzazione:
```cpp
QJsonObject Book::serializza() const {
    QJsonObject jsonObject;
    jsonObject["type"] = "Book";
    jsonObject["title"] = title;
    jsonObject["year"] = year;
    jsonObject["author"] = author;
    jsonObject["isbn"] = isbn;
    jsonObject["publisher"] = publisher;
    jsonObject["coverImagePath"] = coverImagePath;
    return jsonObject;
}
```

### Gestione File
- **JsonSerializer**: Classe dedicata per I/O
- **Dialog File**: Interfaccia utente per salvataggio/caricamento
- **Gestione Errori**: Validazione e messaggi di errore

## Sistema di Autenticazione

### Componenti
- **User**: Classe per gestione credenziali
- **UserAuthenticator**: Autenticazione con hash SHA-256
- **LoginDialog**: Interfaccia di login con stile coerente

### Sicurezza
- Hash delle password con SHA-256
- Validazione input utente
- Gestione sessioni persistenti

## Gestione Immagini

### Funzionalità
- **Selezione File**: Dialog per scelta immagini
- **Anteprima**: Visualizzazione ridimensionata
- **Formati Supportati**: PNG, JPG, JPEG, GIF
- **Fallback**: Testo placeholder se immagine mancante

### Integrazione con Visitor
```cpp
QWidget* MediaWidgetVisitor::createBaseWidget(const QString& title, int year, const QString& coverImagePath) {
    // ... creazione widget base
    
    QLabel* coverLabel = new QLabel();
    if (!coverImagePath.isEmpty() && QFileInfo::exists(coverImagePath)) {
        QPixmap pixmap(coverImagePath);
        coverLabel->setPixmap(pixmap.scaled(coverLabel->size(), Qt::KeepAspectRatio));
    } else {
        coverLabel->setText("Nessuna\nImmagine");
    }
    
    // ... resto del layout
}
```

## Vantaggi dell'Architettura

### Pattern Visitor
1. **Estensibilità**: Facile aggiunta di nuovi tipi di visualizzazione
2. **Separazione**: Logica di presentazione separata dal modello
3. **Riutilizzabilità**: Visitor utilizzabili in contesti diversi
4. **Manutenibilità**: Modifiche localizzate

### Esempi di Estensione
```cpp
// Visitor per esportazione
class MediaExportVisitor : public MediaVisitor {
    QWidget* visit(Book* book) override {
        // Genera widget per esportazione PDF
    }
};

// Visitor per statistiche
class MediaStatsVisitor : public MediaVisitor {
    QWidget* visit(Book* book) override {
        // Genera widget con grafici statistici
    }
};
```

### Interfaccia Grafica
1. **Usabilità**: Interfaccia intuitiva e responsive
2. **Feedback**: Indicazioni visive immediate
3. **Consistenza**: Design uniforme in tutta l'applicazione
4. **Accessibilità**: Supporto keyboard e mouse

## Testing e Validazione

### Test di Compilazione
- ✅ Compilazione senza errori o warning
- ✅ Linking corretto di tutte le dipendenze
- ✅ Generazione eseguibile funzionante

### Test Funzionali
- ✅ Login con credenziali corrette/errate
- ✅ Operazioni CRUD per tutti i tipi di media
- ✅ Persistenza e caricamento dati JSON
- ✅ Gestione immagini e anteprima
- ✅ Ricerca e filtri per tipo

### Test del Pattern Visitor
- ✅ Generazione widget specifici per ogni tipo
- ✅ Visualizzazione corretta informazioni
- ✅ Gestione click e selezione
- ✅ Aggiornamento dinamico interfaccia

## Confronto con Implementazioni Alternative

### Senza Pattern Visitor
```cpp
// Approccio tradizionale (meno flessibile)
QWidget* createMediaWidget(Media* media) {
    if (Book* book = dynamic_cast<Book*>(media)) {
        // Logica specifica per libri
    } else if (Film* film = dynamic_cast<Film*>(media)) {
        // Logica specifica per film
    }
    // Difficile da estendere e mantenere
}
```

### Con Pattern Visitor
```cpp
// Approccio con Visitor (più flessibile)
QWidget* widget = media->accept(*widgetVisitor);
// Estensibile, manutenibile, riutilizzabile
```

## Metriche del Progetto

### Linee di Codice
- **Totale**: ~2000 linee
- **Modello**: ~800 linee
- **Vista**: ~1000 linee
- **Persistenza**: ~200 linee

### Classi Implementate
- **Modello**: 7 classi principali
- **Vista**: 5 classi GUI
- **Visitor**: 1 interfaccia + 1 implementazione
- **Utilità**: 3 classi di supporto

### File di Progetto
- **Header**: 12 file .h
- **Implementazione**: 12 file .cpp
- **Progetto Qt**: 1 file .pro
- **Documentazione**: 3 file markdown

## Conclusioni

### Obiettivi Raggiunti
✅ **Pattern Visitor**: Utilizzato correttamente per generazione widget GUI  
✅ **Separazione Architetturale**: Modello logico indipendente dalla GUI  
✅ **Polimorfismo**: Implementato con metodi virtuali e factory pattern  
✅ **Interfaccia Qt**: Layout moderno e intuitivo  
✅ **Funzionalità Complete**: CRUD, login, persistenza, immagini  
✅ **Estensibilità**: Architettura facilmente estendibile  

### Punti di Forza
1. **Uso Corretto dei Pattern**: Visitor utilizzato per il suo scopo originale
2. **Interfaccia Moderna**: Design intuitivo e responsive
3. **Codice Manutenibile**: Architettura ben strutturata
4. **Funzionalità Complete**: Tutte le operazioni richieste implementate

### Possibili Miglioramenti Futuri
1. **Visitor Aggiuntivi**: Export PDF, statistiche, stampa
2. **Filtri Avanzati**: Per autore, genere, anno
3. **Gestione Utenti**: Ruoli e permessi differenziati
4. **Database**: Migrazione da JSON a database relazionale

### Valore Didattico
Il progetto dimostra:
- Comprensione approfondita del pattern Visitor
- Capacità di progettazione architetturale
- Competenze nell'uso di Qt per GUI
- Applicazione pratica dei principi OOP

L'implementazione finale rappresenta un esempio completo di applicazione desktop moderna che utilizza correttamente i pattern di design per creare un'architettura robusta, estensibile e manutenibile.

