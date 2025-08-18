#include "MediaWidgetVisitor.h"
#include "../model/Book.h"
#include "../model/Film.h"
#include "../model/MagazineArticle.h"
#include <QFrame>
#include <QFont>

MediaWidgetVisitor::MediaWidgetVisitor(bool isEditMode)
    : editMode(isEditMode), currentWidget(nullptr),
      titleEdit(nullptr), yearSpin(nullptr), coverImageEdit(nullptr), browseImageBtn(nullptr),
      authorEdit(nullptr), isbnEdit(nullptr), publisherEdit(nullptr),
      directorEdit(nullptr), durationSpin(nullptr), genreEdit(nullptr),
      articleAuthorEdit(nullptr), magazineEdit(nullptr), doiEdit(nullptr) {}

MediaWidgetVisitor::~MediaWidgetVisitor() {}

QWidget *MediaWidgetVisitor::visit(Book *book)
{
    if (editMode)
    {
        // Modalità edit - crea form di modifica
        currentWidget = new QWidget();
        currentWidget->setFixedSize(400, 500);

        QVBoxLayout *mainLayout = new QVBoxLayout(currentWidget);
        QFormLayout *formLayout = new QFormLayout();

        // Campi comuni
        createCommonFields(formLayout);

        // Campi specifici per Book
        authorEdit = new QLineEdit();
        isbnEdit = new QLineEdit();
        publisherEdit = new QLineEdit();

        formLayout->addRow("Autore:", authorEdit);
        formLayout->addRow("ISBN:", isbnEdit);
        formLayout->addRow("Editore:", publisherEdit);

        mainLayout->addLayout(formLayout);

        // Se c'è un book esistente, popola i campi
        if (book)
        {
            populateFields(book);
            authorEdit->setText(book->getAuthor());
            isbnEdit->setText(book->getIsbn());
            publisherEdit->setText(book->getPublisher());
        }
    }
    else
    {
        // Modalità display - crea widget di visualizzazione
        currentWidget = createBaseWidget(book->getTitle(), book->getYear(), book->getCoverImagePath());
        addBookSpecificInfo(currentWidget, book->getAuthor(), book->getIsbn(), book->getPublisher());
    }
    return currentWidget;
}

QWidget *MediaWidgetVisitor::visit(Film *film)
{
    if (editMode)
    {
        // Modalità edit - crea form di modifica
        currentWidget = new QWidget();
        currentWidget->setFixedSize(400, 500);

        QVBoxLayout *mainLayout = new QVBoxLayout(currentWidget);
        QFormLayout *formLayout = new QFormLayout();

        // Campi comuni
        createCommonFields(formLayout);

        // Campi specifici per Film
        directorEdit = new QLineEdit();
        durationSpin = new QSpinBox();
        durationSpin->setRange(1, 600);
        durationSpin->setSuffix(" min");
        genreEdit = new QLineEdit();

        formLayout->addRow("Regista:", directorEdit);
        formLayout->addRow("Durata:", durationSpin);
        formLayout->addRow("Genere:", genreEdit);

        mainLayout->addLayout(formLayout);

        // Se c'è un film esistente, popola i campi
        if (film)
        {
            populateFields(film);
            directorEdit->setText(film->getDirector());
            durationSpin->setValue(film->getDuration());
            genreEdit->setText(film->getGenre());
        }
    }
    else
    {
        // Modalità display - crea widget di visualizzazione
        currentWidget = createBaseWidget(film->getTitle(), film->getYear(), film->getCoverImagePath());
        addFilmSpecificInfo(currentWidget, film->getDirector(), film->getDuration(), film->getGenre());
    }
    return currentWidget;
}

QWidget *MediaWidgetVisitor::visit(MagazineArticle *article)
{
    if (editMode)
    {
        // Modalità edit - crea form di modifica
        currentWidget = new QWidget();
        currentWidget->setFixedSize(400, 500);

        QVBoxLayout *mainLayout = new QVBoxLayout(currentWidget);
        QFormLayout *formLayout = new QFormLayout();

        // Campi comuni
        createCommonFields(formLayout);

        // Campi specifici per MagazineArticle
        articleAuthorEdit = new QLineEdit();
        magazineEdit = new QLineEdit();
        doiEdit = new QLineEdit();

        formLayout->addRow("Autore Articolo:", articleAuthorEdit);
        formLayout->addRow("Rivista:", magazineEdit);
        formLayout->addRow("DOI:", doiEdit);

        mainLayout->addLayout(formLayout);

        // Se c'è un articolo esistente, popola i campi
        if (article)
        {
            populateFields(article);
            articleAuthorEdit->setText(article->getAuthor());
            magazineEdit->setText(article->getMagazine());
            doiEdit->setText(article->getDoi());
        }
    }
    else
    {
        // Modalità display - crea widget di visualizzazione
        currentWidget = createBaseWidget(article->getTitle(), article->getYear(), article->getCoverImagePath());
        addMagazineArticleSpecificInfo(currentWidget, article->getAuthor(), article->getMagazine(), article->getDoi());
    }
    return currentWidget;
}

QWidget *MediaWidgetVisitor::getWidget() const
{
    return currentWidget;
}

QWidget *MediaWidgetVisitor::createBaseWidget(const QString &title, int year, const QString &coverImagePath)
{
    QWidget *widget = new QWidget();
    widget->setFixedSize(300, 400);
    widget->setStyleSheet("QWidget { background-color: white; border: 2px solid #ddd; border-radius: 8px; }");

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setSpacing(8);
    layout->setContentsMargins(10, 10, 10, 10);

    // Cover image
    QLabel *coverLabel = new QLabel();
    coverLabel->setFixedSize(120, 160);
    coverLabel->setScaledContents(true);
    coverLabel->setStyleSheet("border: 1px solid #ccc; background-color: #f9f9f9;");
    coverLabel->setAlignment(Qt::AlignCenter);

    if (!coverImagePath.isEmpty() && QFileInfo::exists(coverImagePath))
    {
        QPixmap pixmap(coverImagePath);
        coverLabel->setPixmap(pixmap.scaled(coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        coverLabel->setText("Nessuna\nImmagine");
        coverLabel->setStyleSheet(coverLabel->styleSheet() + " color: #666;");
    }

    // Title
    QLabel *titleLabel = new QLabel(title);
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    titleLabel->setFont(titleFont);
    titleLabel->setWordWrap(true);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #333; padding: 5px;");

    // Year
    QLabel *yearLabel = new QLabel(QString("Anno: %1").arg(year));
    yearLabel->setAlignment(Qt::AlignCenter);
    yearLabel->setStyleSheet("color: #666; font-size: 10px;");

    // Add to layout
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addStretch();
    imageLayout->addWidget(coverLabel);
    imageLayout->addStretch();

    layout->addLayout(imageLayout);
    layout->addWidget(titleLabel);
    layout->addWidget(yearLabel);

    return widget;
}

void MediaWidgetVisitor::addBookSpecificInfo(QWidget *widget, const QString &author, const QString &isbn, const QString &publisher)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(widget->layout());
    if (!layout)
        return;

    // Type indicator
    QLabel *typeLabel = new QLabel("📚 LIBRO");
    typeLabel->setAlignment(Qt::AlignCenter);
    typeLabel->setStyleSheet("background-color: #4CAF50; color: white; padding: 4px; border-radius: 4px; font-weight: bold;");
    layout->insertWidget(0, typeLabel);

    // Author
    QLabel *authorLabel = new QLabel(QString("Autore: %1").arg(author));
    authorLabel->setWordWrap(true);
    authorLabel->setStyleSheet("color: #444; font-size: 10px; padding: 2px;");
    layout->addWidget(authorLabel);

    // Publisher
    QLabel *publisherLabel = new QLabel(QString("Editore: %1").arg(publisher));
    publisherLabel->setWordWrap(true);
    publisherLabel->setStyleSheet("color: #444; font-size: 10px; padding: 2px;");
    layout->addWidget(publisherLabel);

    // ISBN
    QLabel *isbnLabel = new QLabel(QString("ISBN: %1").arg(isbn));
    isbnLabel->setWordWrap(true);
    isbnLabel->setStyleSheet("color: #666; font-size: 9px; padding: 2px;");
    layout->addWidget(isbnLabel);

    layout->addStretch();
}

void MediaWidgetVisitor::addFilmSpecificInfo(QWidget *widget, const QString &director, int duration, const QString &genre)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(widget->layout());
    if (!layout)
        return;

    // Type indicator
    QLabel *typeLabel = new QLabel("🎬 FILM");
    typeLabel->setAlignment(Qt::AlignCenter);
    typeLabel->setStyleSheet("background-color: #2196F3; color: white; padding: 4px; border-radius: 4px; font-weight: bold;");
    layout->insertWidget(0, typeLabel);

    // Director
    QLabel *directorLabel = new QLabel(QString("Regista: %1").arg(director));
    directorLabel->setWordWrap(true);
    directorLabel->setStyleSheet("color: #444; font-size: 10px; padding: 2px;");
    layout->addWidget(directorLabel);

    // Duration
    QLabel *durationLabel = new QLabel(QString("Durata: %1 min").arg(duration));
    durationLabel->setStyleSheet("color: #444; font-size: 10px; padding: 2px;");
    layout->addWidget(durationLabel);

    // Genre
    QLabel *genreLabel = new QLabel(QString("Genere: %1").arg(genre));
    genreLabel->setWordWrap(true);
    genreLabel->setStyleSheet("color: #666; font-size: 9px; padding: 2px;");
    layout->addWidget(genreLabel);

    layout->addStretch();
}

void MediaWidgetVisitor::addMagazineArticleSpecificInfo(QWidget *widget, const QString &author, const QString &magazine, const QString &doi)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(widget->layout());
    if (!layout)
        return;

    // Type indicator
    QLabel *typeLabel = new QLabel("📄 ARTICOLO");
    typeLabel->setAlignment(Qt::AlignCenter);
    typeLabel->setStyleSheet("background-color: #FF9800; color: white; padding: 4px; border-radius: 4px; font-weight: bold;");
    layout->insertWidget(0, typeLabel);

    // Author
    QLabel *authorLabel = new QLabel(QString("Autore: %1").arg(author));
    authorLabel->setWordWrap(true);
    authorLabel->setStyleSheet("color: #444; font-size: 10px; padding: 2px;");
    layout->addWidget(authorLabel);

    // Magazine
    QLabel *magazineLabel = new QLabel(QString("Rivista: %1").arg(magazine));
    magazineLabel->setWordWrap(true);
    magazineLabel->setStyleSheet("color: #444; font-size: 10px; padding: 2px;");
    layout->addWidget(magazineLabel);

    // DOI
    QLabel *doiLabel = new QLabel(QString("DOI: %1").arg(doi));
    doiLabel->setWordWrap(true);
    doiLabel->setStyleSheet("color: #666; font-size: 9px; padding: 2px;");
    layout->addWidget(doiLabel);

    layout->addStretch();
}

// Metodi per la modalità edit
void MediaWidgetVisitor::createCommonFields(QFormLayout *layout)
{
    titleEdit = new QLineEdit();
    yearSpin = new QSpinBox();
    yearSpin->setRange(1000, 2100);
    coverImageEdit = new QLineEdit();

    layout->addRow("Titolo:", titleEdit);
    layout->addRow("Anno:", yearSpin);

    // Setup per selezione immagine
    QHBoxLayout *imageLayout = new QHBoxLayout();
    browseImageBtn = new QPushButton("Sfoglia...");
    imageLayout->addWidget(coverImageEdit);
    imageLayout->addWidget(browseImageBtn);

    QWidget *imageWidget = new QWidget();
    imageWidget->setLayout(imageLayout);
    layout->addRow("Immagine Copertina:", imageWidget);

    setupImageSelection();
}

void MediaWidgetVisitor::setupImageSelection()
{
    if (browseImageBtn)
    {
        connect(browseImageBtn, &QPushButton::clicked, this, &MediaWidgetVisitor::selectCoverImage);
    }
}

void MediaWidgetVisitor::selectCoverImage()
{
    QString fileName = QFileDialog::getOpenFileName(currentWidget,
                                                    "Seleziona immagine di copertina", "", "Immagini (*.png *.jpg *.jpeg *.gif *.bmp)");
    if (!fileName.isEmpty() && coverImageEdit)
    {
        coverImageEdit->setText(fileName);
    }
}

QString MediaWidgetVisitor::getTitle() const
{
    return titleEdit ? titleEdit->text() : QString();
}

int MediaWidgetVisitor::getYear() const
{
    return yearSpin ? yearSpin->value() : 0;
}

QString MediaWidgetVisitor::getCoverImagePath() const
{
    return coverImageEdit ? coverImageEdit->text() : QString();
}

QString MediaWidgetVisitor::getAuthor() const
{
    return authorEdit ? authorEdit->text() : QString();
}

QString MediaWidgetVisitor::getIsbn() const
{
    return isbnEdit ? isbnEdit->text() : QString();
}

QString MediaWidgetVisitor::getPublisher() const
{
    return publisherEdit ? publisherEdit->text() : QString();
}

QString MediaWidgetVisitor::getDirector() const
{
    return directorEdit ? directorEdit->text() : QString();
}

int MediaWidgetVisitor::getDuration() const
{
    return durationSpin ? durationSpin->value() : 0;
}

QString MediaWidgetVisitor::getGenre() const
{
    return genreEdit ? genreEdit->text() : QString();
}

QString MediaWidgetVisitor::getArticleAuthor() const
{
    return articleAuthorEdit ? articleAuthorEdit->text() : QString();
}

QString MediaWidgetVisitor::getMagazine() const
{
    return magazineEdit ? magazineEdit->text() : QString();
}

QString MediaWidgetVisitor::getDoi() const
{
    return doiEdit ? doiEdit->text() : QString();
}

void MediaWidgetVisitor::populateFields(Media *media)
{
    if (!media)
        return;

    if (titleEdit)
        titleEdit->setText(media->getTitle());
    if (yearSpin)
        yearSpin->setValue(media->getYear());
    if (coverImageEdit)
        coverImageEdit->setText(media->getCoverImagePath());
}
