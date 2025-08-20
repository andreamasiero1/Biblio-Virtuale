#include "MainWindow.h"
#include "../model/Book.h"
#include "../model/Film.h"
#include "../model/MagazineArticle.h"
#include "../persistence/JsonSerializer.h"
#include <QPixmap>
#include <QFileInfo>
#include <QMouseEvent>
#include <QRegularExpression>
#include <QCoreApplication>
#include <QDir>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), selectedMedia(nullptr)
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    loadDefaultLibrary(); // Carica la biblioteca di esempio se esiste
    updateMediaDisplay();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Toolbar with filter and action buttons
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    // Media type filter
    QLabel *filterLabel = new QLabel("Tipo Media:");
    QComboBox *mediaTypeFilter = new QComboBox();
    mediaTypeFilter->addItem("Tutti", "all");
    mediaTypeFilter->addItem("📚 Libri", "book");
    mediaTypeFilter->addItem("🎬 Film", "film");
    mediaTypeFilter->addItem("📄 Articoli", "article");

    // Search box
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Cerca media...");
    QPushButton *searchBtn = new QPushButton("🔍 Cerca");

    // Action buttons
    QPushButton *addBtn = new QPushButton("➕ Aggiungi");
    QPushButton *editBtn = new QPushButton("✏️ Modifica");
    QPushButton *deleteBtn = new QPushButton("🗑️ Elimina");
    QPushButton *detailsBtn = new QPushButton("👁️ Dettagli");

    toolbarLayout->addWidget(filterLabel);
    toolbarLayout->addWidget(mediaTypeFilter);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(searchEdit);
    toolbarLayout->addWidget(searchBtn);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(addBtn);
    toolbarLayout->addWidget(editBtn);
    toolbarLayout->addWidget(deleteBtn);
    toolbarLayout->addWidget(detailsBtn);

    // Media display area
    QScrollArea *scrollArea = new QScrollArea();
    QWidget *contentWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(contentWidget);

    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(600);

    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(scrollArea);

    // Store references for later use
    this->mediaTypeFilter = mediaTypeFilter;
    this->searchEdit = searchEdit;
    this->gridLayout = gridLayout;
    this->contentWidget = contentWidget;
    this->scrollArea = scrollArea;

    // Connect signals
    connect(mediaTypeFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFilterChanged);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchMedia);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::searchMedia);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addMedia);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editMedia);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteMedia);
    connect(detailsBtn, &QPushButton::clicked, this, &MainWindow::showMediaDetails);

    setWindowTitle("Biblioteca Virtuale");
    resize(1200, 800);
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();

    QMenu *fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&Carica Biblioteca", this, &MainWindow::loadLibrary, QKeySequence::Open);
    fileMenu->addAction("&Salva Biblioteca", this, &MainWindow::saveLibrary, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("&Esci", this, &QWidget::close, QKeySequence::Quit);

    QMenu *mediaMenu = menuBar->addMenu("&Media");
    mediaMenu->addAction("&Aggiungi", this, &MainWindow::addMedia, QKeySequence::New);
    mediaMenu->addAction("&Modifica", this, &MainWindow::editMedia);
    mediaMenu->addAction("&Elimina", this, &MainWindow::deleteMedia, QKeySequence::Delete);
    mediaMenu->addSeparator();
    mediaMenu->addAction("&Cerca", this, &MainWindow::searchMedia, QKeySequence::Find);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Pronto", 2000);
}

void MainWindow::updateMediaDisplay()
{
    clearMediaDisplay();

    QList<Media *> mediaToDisplay = getFilteredMedia();
    populateDisplayWithMedia(mediaToDisplay);
}

void MainWindow::clearMediaDisplay()
{
    clearLayout(gridLayout);
}

QList<Media *> MainWindow::getFilteredMedia() const
{
    QString currentFilter = mediaTypeFilter->currentData().toString();
    QString searchTerm = searchEdit->text().trimmed();

    QList<Media *> allMedia;

    // Get all media based on type filter using Visitor pattern
    if (currentFilter == "all")
    {
        allMedia = biblioteca.getTuttiMedia();
    }
    else if (currentFilter == "book")
    {
        allMedia = biblioteca.collectMediaByType(MediaFilter::FilterType::BOOKS_ONLY);
    }
    else if (currentFilter == "film")
    {
        allMedia = biblioteca.collectMediaByType(MediaFilter::FilterType::FILMS_ONLY);
    }
    else if (currentFilter == "article")
    {
        allMedia = biblioteca.collectMediaByType(MediaFilter::FilterType::ARTICLES_ONLY);
    }

    // Apply search filter if present
    if (!searchTerm.isEmpty())
    {
        QList<Media *> searchResults;
        for (Media *media : allMedia)
        {
            if (media->getTitle().contains(searchTerm, Qt::CaseInsensitive))
            {
                searchResults.append(media);
            }
        }
        return searchResults;
    }

    return allMedia;
}

void MainWindow::populateDisplayWithMedia(const QList<Media *> &mediaList)
{
    int columns = 4; // Media per riga
    int currentRow = gridLayout->rowCount();
    int currentCol = 0;

    for (Media *media : mediaList)
    {
        addMediaWidgetToGrid(media, currentRow, currentCol);

        currentCol++;
        if (currentCol >= columns)
        {
            currentCol = 0;
            currentRow++;
        }
    }
}

void MainWindow::addMediaWidgetToGrid(Media *media, int row, int col)
{
    // Usa il visitor pattern come richiesto
    MediaVisitor *visitor = new MediaWidgetVisitor();
    media->accept(*visitor);
    QWidget *widget = (static_cast<MediaWidgetVisitor *>(visitor))->getWidget();

    widget->setProperty("media", QVariant::fromValue(static_cast<void *>(media)));
    widget->setProperty("clickable", true);
    widget->installEventFilter(this);

    gridLayout->addWidget(widget, row, col);

    // Cleanup
    delete visitor;
}
void MainWindow::clearLayout(QLayout *layout)
{
    if (!layout)
        return;

    while (QLayoutItem *item = layout->takeAt(0))
    {
        if (QWidget *widget = item->widget())
        {
            // Rimuovi tutti gli event filter dal widget
            widget->removeEventFilter(this);

            // Rimuovi il property "media" per evitare puntatori invalidi
            widget->setProperty("media", QVariant());
            widget->setProperty("clickable", QVariant());

            // Disconnetti tutti i segnali del widget
            disconnect(widget, nullptr, this, nullptr);

            widget->deleteLater();
        }
        delete item;
    }
}

void MainWindow::onFilterChanged()
{
    updateMediaDisplay();
    selectedMedia = nullptr; // Clear selection when filter changes
}

void MainWindow::showMediaDetails()
{
    if (!selectedMedia)
    {
        QMessageBox::information(this, "Informazione", "Seleziona un media per visualizzare i dettagli.");
        return;
    }

    QString details = selectedMedia->visualizzaDettagli();
    QMessageBox::information(this, "Dettagli Media", details);
}

void MainWindow::addMedia()
{
    // Dialog per selezionare il tipo di media
    QStringList mediaTypes;
    mediaTypes << "Libro" << "Film" << "Articolo di Rivista";

    bool ok;
    QString selectedType = QInputDialog::getItem(this, "Aggiungi Media",
                                                 "Seleziona il tipo di media:",
                                                 mediaTypes, 0, false, &ok);

    if (!ok || selectedType.isEmpty())
    {
        return;
    }

    // Crea il media temporaneo per il visitor
    Media *tempMedia = nullptr;
    if (selectedType == "Libro")
    {
        tempMedia = new Book("", 2024, "", "", "", "");
    }
    else if (selectedType == "Film")
    {
        tempMedia = new Film("", 2024, "", 0, "", "");
    }
    else if (selectedType == "Articolo di Rivista")
    {
        tempMedia = new MagazineArticle("", 2024, "", "", "", "");
    }

    if (!tempMedia)
    {
        return;
    }

    // Usa il visitor pattern come richiesto
    MediaVisitor *visitor = new MediaWidgetVisitor(true);
    tempMedia->accept(*visitor);
    QWidget *widget = (static_cast<MediaWidgetVisitor *>(visitor))->getWidget();

    // Crea il dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Aggiungi " + selectedType);
    dialog.setModal(true);

    QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);
    dialogLayout->addWidget(widget);

    // Bottoni
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("Aggiungi");
    QPushButton *cancelBtn = new QPushButton("Annulla");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    dialogLayout->addLayout(buttonLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        // Crea il nuovo media con i dati inseriti
        MediaWidgetVisitor *editVisitor = static_cast<MediaWidgetVisitor *>(visitor);

        Media *newMedia = nullptr;
        if (selectedType == "Libro")
        {
            newMedia = new Book(editVisitor->getTitle(), editVisitor->getYear(),
                                editVisitor->getAuthor(), editVisitor->getIsbn(),
                                editVisitor->getPublisher(), editVisitor->getCoverImagePath());
        }
        else if (selectedType == "Film")
        {
            newMedia = new Film(editVisitor->getTitle(), editVisitor->getYear(),
                                editVisitor->getDirector(), editVisitor->getDuration(),
                                editVisitor->getGenre(), editVisitor->getCoverImagePath());
        }
        else if (selectedType == "Articolo di Rivista")
        {
            newMedia = new MagazineArticle(editVisitor->getTitle(), editVisitor->getYear(),
                                           editVisitor->getArticleAuthor(), editVisitor->getMagazine(),
                                           editVisitor->getDoi(), editVisitor->getCoverImagePath());
        }

        if (newMedia)
        {
            biblioteca.aggiungiMedia(newMedia);
            updateMediaDisplay();
            statusBar()->showMessage("Media aggiunto con successo", 2000);
        }
    }

    // Cleanup
    delete tempMedia;
    delete visitor;
}

void MainWindow::editMedia()
{
    if (!selectedMedia)
    {
        QMessageBox::warning(this, "Attenzione", "Seleziona un media da modificare.");
        return;
    }

    // --- VALIDAZIONE AGGIUNTIVA ---
    // Verifica che il media selezionato sia ancora valido nella biblioteca attuale
    QList<Media *> currentMedia = biblioteca.getTuttiMedia();
    if (!currentMedia.contains(selectedMedia))
    {
        QMessageBox::critical(this, "Errore", "L'elemento selezionato non è più valido. Prova a selezionarlo di nuovo.");
        selectedMedia = nullptr;
        updateMediaDisplay();
        return;
    }
    // --- FINE VALIDAZIONE ---

    // Usa il visitor pattern come richiesto
    MediaVisitor *visitor = new MediaWidgetVisitor(true);
    selectedMedia->accept(*visitor);
    QWidget *widget = (static_cast<MediaWidgetVisitor *>(visitor))->getWidget();

    // Popola i campi con i dati esistenti
    MediaWidgetVisitor *editVisitor = static_cast<MediaWidgetVisitor *>(visitor);
    editVisitor->populateFields(selectedMedia);

    // Crea il dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Modifica Media");
    dialog.setModal(true);

    QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);
    dialogLayout->addWidget(widget);

    // Bottoni
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("Salva");
    QPushButton *cancelBtn = new QPushButton("Annulla");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    dialogLayout->addLayout(buttonLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        // Aggiorna il media esistente
        selectedMedia->setTitle(editVisitor->getTitle());
        selectedMedia->setYear(editVisitor->getYear());
        selectedMedia->setCoverImagePath(editVisitor->getCoverImagePath());

        // Aggiorna i campi specifici del tipo
        if (Book *book = dynamic_cast<Book *>(selectedMedia))
        {
            book->setAuthor(editVisitor->getAuthor());
            book->setIsbn(editVisitor->getIsbn());
            book->setPublisher(editVisitor->getPublisher());
        }
        else if (Film *film = dynamic_cast<Film *>(selectedMedia))
        {
            film->setDirector(editVisitor->getDirector());
            film->setDuration(editVisitor->getDuration());
            film->setGenre(editVisitor->getGenre());
        }
        else if (MagazineArticle *article = dynamic_cast<MagazineArticle *>(selectedMedia))
        {
            article->setAuthor(editVisitor->getArticleAuthor());
            article->setMagazine(editVisitor->getMagazine());
            article->setDoi(editVisitor->getDoi());
        }

        updateMediaDisplay();
        statusBar()->showMessage("Media modificato con successo", 2000);
    }

    // Cleanup
    delete visitor;
}

void MainWindow::deleteMedia()
{
    if (!selectedMedia)
    {
        QMessageBox::warning(this, "Attenzione", "Seleziona un media da eliminare.");
        return;
    }

    // --- VALIDAZIONE AGGIUNTIVA ---
    // Verifica che il media selezionato sia ancora valido nella biblioteca attuale
    QList<Media *> currentMedia = biblioteca.getTuttiMedia();
    if (!currentMedia.contains(selectedMedia))
    {
        QMessageBox::critical(this, "Errore", "L'elemento selezionato non è più valido. Prova a selezionarlo di nuovo.");
        selectedMedia = nullptr;
        updateMediaDisplay();
        return;
    }
    // --- FINE VALIDAZIONE ---

    int ret = QMessageBox::question(this, "Conferma",
                                    QString("Sei sicuro di voler eliminare \"%1\"?").arg(selectedMedia->getTitle()),
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        biblioteca.rimuoviMedia(selectedMedia);
        selectedMedia = nullptr;
        updateMediaDisplay();
        statusBar()->showMessage("Media eliminato con successo", 2000);
    }
}

void MainWindow::searchMedia()
{
    updateMediaDisplay(); // Il filtro di ricerca è già gestito in getFilteredMedia()

    QString searchTerm = searchEdit->text().trimmed();
    if (!searchTerm.isEmpty())
    {
        QList<Media *> results = getFilteredMedia();
        statusBar()->showMessage(QString("Trovati %1 risultati per \"%2\"").arg(results.size()).arg(searchTerm), 3000);
    }
    else
    {
        statusBar()->showMessage("Visualizzati tutti i media", 2000);
    }
}

void MainWindow::saveLibrary()
{
    // Determina la cartella di default per i salvataggi
    QString defaultDir = QCoreApplication::applicationDirPath() + "/data/bibliotecas";
    QDir().mkpath(defaultDir); // Crea la cartella se non esiste

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Salva Biblioteca",
                                                    defaultDir + "/biblioteca.json",
                                                    "JSON Files (*.json);;All Files (*)");
    if (!fileName.isEmpty())
    {
        // Assicurati che il file abbia l'estensione .json
        if (!fileName.endsWith(".json", Qt::CaseInsensitive))
        {
            fileName += ".json";
        }

        JsonSerializer serializer;
        if (serializer.salvaBiblioteca(biblioteca, fileName))
        {
            statusBar()->showMessage(QString("Biblioteca salvata: %1").arg(QFileInfo(fileName).fileName()), 3000);
            QMessageBox::information(this, "Salvataggio Completato",
                                     QString("La biblioteca è stata salvata con successo in:\n%1").arg(fileName));
        }
        else
        {
            QMessageBox::critical(this, "Errore di Salvataggio",
                                  "Impossibile salvare la biblioteca.\nVerifica i permessi di scrittura del file.");
        }
    }
}

void MainWindow::loadLibrary()
{
    // Determina la cartella di default per i caricamenti
    QString defaultDir = QCoreApplication::applicationDirPath() + "/data/bibliotecas";
    QDir().mkpath(defaultDir); // Crea la cartella se non esiste

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Carica Biblioteca",
                                                    defaultDir,
                                                    "JSON Files (*.json);;All Files (*)");
    if (!fileName.isEmpty())
    {
        // Conferma se la biblioteca attuale ha dei media
        if (biblioteca.dimensione() > 0)
        {
            int ret = QMessageBox::question(this, "Conferma Caricamento",
                                            QString("La biblioteca attuale contiene %1 elementi.\n"
                                                    "Caricando una nuova biblioteca, tutti i dati attuali andranno persi.\n\n"
                                                    "Vuoi continuare?")
                                                .arg(biblioteca.dimensione()),
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                            QMessageBox::No);

            if (ret != QMessageBox::Yes)
            {
                return; // L'utente ha annullato o scelto No
            }
        }

        try
        {
            JsonSerializer serializer;
            Biblioteca loadedLibrary;

            // Usa la versione con eccezioni per un controllo migliore
            serializer.caricaBibliotecaThrows(loadedLibrary, fileName);

            // Prima pulisci tutto completamente
            selectedMedia = nullptr;
            clearMediaDisplay();

            // Forza l'elaborazione degli eventi per assicurarsi che i widget siano completamente distrutti
            QCoreApplication::processEvents();

            // Ora carica la nuova biblioteca
            biblioteca = loadedLibrary;

            // Attendi un momento prima di aggiornare il display
            QTimer::singleShot(100, this, [this]()
                               { updateMediaDisplay(); });

            statusBar()->showMessage(QString("Biblioteca caricata: %1 elementi").arg(biblioteca.dimensione()), 3000);
            QMessageBox::information(this, "Caricamento Completato",
                                     QString("La biblioteca è stata caricata con successo!\n"
                                             "Elementi caricati: %1\n"
                                             "File: %2")
                                         .arg(biblioteca.dimensione())
                                         .arg(QFileInfo(fileName).fileName()));
        }
        catch (const FileNotFoundException &e)
        {
            QMessageBox::critical(this, "File non trovato",
                                  QString("Il file selezionato non può essere aperto:\n%1")
                                      .arg(QString::fromStdString(e.what())));
        }
        catch (const JsonParseException &e)
        {
            QMessageBox::critical(this, "Errore nel formato del file",
                                  QString("Il file JSON non è valido o è corrotto:\n%1")
                                      .arg(QString::fromStdString(e.what())));
        }
        catch (const BibliotecaException &e)
        {
            QMessageBox::critical(this, "Errore di caricamento",
                                  QString("Si è verificato un errore durante il caricamento:\n%1")
                                      .arg(QString::fromStdString(e.what())));
        }
        catch (...)
        {
            QMessageBox::critical(this, "Errore imprevisto",
                                  "Si è verificato un errore imprevisto durante il caricamento della biblioteca.");
        }
    }
}

void MainWindow::onMediaWidgetClicked()
{
    QWidget *clickedWidget = qobject_cast<QWidget *>(sender());
    if (clickedWidget)
    {
        QVariant mediaVariant = clickedWidget->property("media");
        if (mediaVariant.isValid() && !mediaVariant.isNull())
        {
            Media *media = static_cast<Media *>(mediaVariant.value<void *>());

            // Verifica che il media sia ancora valido nella biblioteca
            QList<Media *> currentMedia = biblioteca.getTuttiMedia();
            if (media && currentMedia.contains(media))
            {
                selectedMedia = media;

                // Update visual selection for all widgets in grid
                for (int i = 0; i < gridLayout->count(); ++i)
                {
                    QWidget *widget = gridLayout->itemAt(i)->widget();
                    if (widget)
                    {
                        if (widget == clickedWidget)
                        {
                            widget->setStyleSheet(widget->styleSheet() + " border: 3px solid #2196F3;");
                        }
                        else
                        {
                            QString style = widget->styleSheet();
                            style.remove(QRegularExpression("border: 3px solid #2196F3;"));
                            widget->setStyleSheet(style);
                        }
                    }
                }

                try
                {
                    QString title = media->getTitle();
                    if (!title.isNull())
                    {
                        statusBar()->showMessage(QString("Selezionato: %1").arg(title), 2000);
                    }
                    else
                    {
                        statusBar()->showMessage("Elemento selezionato", 2000);
                    }
                }
                catch (...)
                {
                    statusBar()->showMessage("Elemento selezionato", 2000);
                    qDebug() << "Errore nell'accesso al titolo del media in onMediaWidgetClicked";
                }
            }
            else
            {
                // Media non valido, aggiorna il display
                selectedMedia = nullptr;
                updateMediaDisplay();
                statusBar()->showMessage("Elemento non più valido", 2000);
            }
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QWidget *widget = qobject_cast<QWidget *>(obj);
        if (widget && widget->property("clickable").toBool())
        {
            // Verifica che il property "media" esista e sia valido
            QVariant mediaVariant = widget->property("media");
            if (!mediaVariant.isValid() || mediaVariant.isNull())
            {
                return QMainWindow::eventFilter(obj, event);
            }

            Media *media = static_cast<Media *>(mediaVariant.value<void *>());
            if (!media)
            {
                return QMainWindow::eventFilter(obj, event);
            }

            // Verifica che il media sia ancora presente nella biblioteca
            QList<Media *> currentMedia = biblioteca.getTuttiMedia();
            if (!currentMedia.contains(media))
            {
                // Il media non è più valido, rimuovi la property
                widget->setProperty("media", QVariant());
                widget->setProperty("clickable", QVariant());
                return QMainWindow::eventFilter(obj, event);
            }

            // Verifica che il puntatore sia effettivamente accessibile (test di lettura sicura)
            try
            {
                QString testTitle = media->getTitle();
                if (testTitle.isNull())
                {
                    // Il puntatore non è valido
                    widget->setProperty("media", QVariant());
                    widget->setProperty("clickable", QVariant());
                    return QMainWindow::eventFilter(obj, event);
                }
            }
            catch (...)
            {
                // Errore nell'accesso al Media, rimuovi la property
                widget->setProperty("media", QVariant());
                widget->setProperty("clickable", QVariant());
                return QMainWindow::eventFilter(obj, event);
            }

            selectedMedia = media;

            // Update visual selection for all widgets in grid
            for (int i = 0; i < gridLayout->count(); ++i)
            {
                QWidget *w = gridLayout->itemAt(i)->widget();
                if (w)
                {
                    if (w == widget)
                    {
                        w->setStyleSheet(w->styleSheet() + " border: 3px solid #2196F3;");
                    }
                    else
                    {
                        QString style = w->styleSheet();
                        style.remove(QRegularExpression("border: 3px solid #2196F3;"));
                        w->setStyleSheet(style);
                    }
                }
            }

            // Verifica nuovamente la validità prima di usare il titolo
            if (media && currentMedia.contains(media))
            {
                try
                {
                    // Verifica che il titolo sia accessibile prima di usarlo
                    QString title = media->getTitle();
                    if (!title.isNull())
                    {
                        statusBar()->showMessage(QString("Selezionato: %1").arg(title), 2000);
                    }
                    else
                    {
                        statusBar()->showMessage("Elemento selezionato", 2000);
                    }
                }
                catch (...)
                {
                    // Se c'è un errore nell'accesso al titolo, usa un messaggio generico
                    statusBar()->showMessage("Elemento selezionato", 2000);
                    qDebug() << "Errore nell'accesso al titolo del media selezionato";
                }
            }
        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::loadDefaultLibrary()
{
    // Prova a caricare la biblioteca di esempio se esiste
    QString exampleFile = QCoreApplication::applicationDirPath() + "/data/biblioteca_esempio.json";
    QFileInfo fileInfo(exampleFile);

    if (fileInfo.exists() && fileInfo.isReadable())
    {
        JsonSerializer serializer;
        Biblioteca loadedLibrary;
        if (serializer.caricaBiblioteca(loadedLibrary, exampleFile))
        {
            biblioteca = loadedLibrary;
            selectedMedia = nullptr; // Reset selection
            statusBar()->showMessage(QString("Biblioteca di esempio caricata (%1 elementi)").arg(biblioteca.dimensione()), 2000);
        }
    }
    else
    {
        // Se il file di esempio non esiste, controlliamo se c'è un backup dell'ultima sessione
        QString backupFile = QCoreApplication::applicationDirPath() + "/data/bibliotecas/ultima_sessione.json";
        QFileInfo backupInfo(backupFile);

        if (backupInfo.exists() && backupInfo.isReadable())
        {
            JsonSerializer serializer;
            Biblioteca loadedLibrary;
            if (serializer.caricaBiblioteca(loadedLibrary, backupFile))
            {
                biblioteca = loadedLibrary;
                selectedMedia = nullptr; // Reset selection
                statusBar()->showMessage(QString("Ultima sessione ripristinata (%1 elementi)").arg(biblioteca.dimensione()), 2000);
            }
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Salva automaticamente la sessione prima di chiudere
    if (biblioteca.dimensione() > 0)
    {
        QString backupDir = QCoreApplication::applicationDirPath() + "/data/bibliotecas";
        QDir().mkpath(backupDir);
        QString backupFile = backupDir + "/ultima_sessione.json";

        JsonSerializer serializer;
        if (serializer.salvaBiblioteca(biblioteca, backupFile))
        {
            statusBar()->showMessage("Sessione salvata automaticamente", 1000);
        }
    }

    // Accetta l'evento di chiusura
    event->accept();
}
