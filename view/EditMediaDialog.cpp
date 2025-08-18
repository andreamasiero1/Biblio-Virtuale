#include "EditMediaDialog.h"
#include "../model/Book.h"
#include "../model/Film.h"
#include "../model/MagazineArticle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPixmap>
#include <QFileInfo>

EditMediaDialog::EditMediaDialog(Biblioteca& biblioteca, Media* mediaToEdit, QWidget *parent)
    : QDialog(parent), biblioteca(biblioteca), currentMedia(mediaToEdit) {
    setupUI();
    populateFields();
    setWindowTitle("Modifica Media");
}

EditMediaDialog::~EditMediaDialog() {}

void EditMediaDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Common fields
    QFormLayout *commonFormLayout = new QFormLayout();
    titleEdit = new QLineEdit();
    yearSpinBox = new QSpinBox();
    yearSpinBox->setRange(1000, 2100);
    coverImagePathEdit = new QLineEdit();
    coverImagePathEdit->setReadOnly(true);
    QPushButton *selectImageBtn = new QPushButton("Seleziona Immagine");
    connect(selectImageBtn, &QPushButton::clicked, this, &EditMediaDialog::selectCoverImage);
    coverImagePreview = new QLabel();
    coverImagePreview->setFixedSize(100, 150);
    coverImagePreview->setScaledContents(true);
    coverImagePreview->setStyleSheet("border: 1px solid gray;");

    commonFormLayout->addRow("Titolo:", titleEdit);
    commonFormLayout->addRow("Anno:", yearSpinBox);
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(coverImagePathEdit);
    imageLayout->addWidget(selectImageBtn);
    commonFormLayout->addRow("Copertina:", imageLayout);
    commonFormLayout->addRow("Anteprima:", coverImagePreview);

    mainLayout->addLayout(commonFormLayout);

    // Stacked widget for type-specific fields
    stackedWidget = new QStackedWidget();
    createBookForm();
    createFilmForm();
    createMagazineArticleForm();
    mainLayout->addWidget(stackedWidget);

    // Dialog buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &EditMediaDialog::editMediaConfirmed);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &EditMediaDialog::reject);
    mainLayout->addWidget(buttonBox);
}

void EditMediaDialog::createBookForm() {
    QWidget *bookPage = new QWidget();
    QFormLayout *formLayout = new QFormLayout(bookPage);
    bookAuthorEdit = new QLineEdit();
    bookIsbnEdit = new QLineEdit();
    bookPublisherEdit = new QLineEdit();
    formLayout->addRow("Autore:", bookAuthorEdit);
    formLayout->addRow("ISBN:", bookIsbnEdit);
    formLayout->addRow("Editore:", bookPublisherEdit);
    stackedWidget->addWidget(bookPage);
}

void EditMediaDialog::createFilmForm() {
    QWidget *filmPage = new QWidget();
    QFormLayout *formLayout = new QFormLayout(filmPage);
    filmDirectorEdit = new QLineEdit();
    filmDurationSpinBox = new QSpinBox();
    filmDurationSpinBox->setRange(1, 1000);
    filmGenreEdit = new QLineEdit();
    formLayout->addRow("Regista:", filmDirectorEdit);
    formLayout->addRow("Durata (min):", filmDurationSpinBox);
    formLayout->addRow("Genere:", filmGenreEdit);
    stackedWidget->addWidget(filmPage);
}

void EditMediaDialog::createMagazineArticleForm() {
    QWidget *articlePage = new QWidget();
    QFormLayout *formLayout = new QFormLayout(articlePage);
    articleAuthorEdit = new QLineEdit();
    articleMagazineEdit = new QLineEdit();
    articleDoiEdit = new QLineEdit();
    formLayout->addRow("Autore:", articleAuthorEdit);
    formLayout->addRow("Rivista:", articleMagazineEdit);
    formLayout->addRow("DOI:", articleDoiEdit);
    stackedWidget->addWidget(articlePage);
}

void EditMediaDialog::populateFields() {
    if (!currentMedia) return;

    titleEdit->setText(currentMedia->getTitle());
    yearSpinBox->setValue(currentMedia->getYear());
    selectedCoverImagePath = currentMedia->getCoverImagePath();
    coverImagePathEdit->setText(QFileInfo(selectedCoverImagePath).fileName());
    if (!selectedCoverImagePath.isEmpty()) {
        QPixmap pixmap(selectedCoverImagePath);
        coverImagePreview->setPixmap(pixmap.scaled(coverImagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Determine media type and populate specific fields
    if (Book* book = dynamic_cast<Book*>(currentMedia)) {
        stackedWidget->setCurrentIndex(0); // Book is at index 0
        bookAuthorEdit->setText(book->getAuthor());
        bookIsbnEdit->setText(book->getIsbn());
        bookPublisherEdit->setText(book->getPublisher());
    } else if (Film* film = dynamic_cast<Film*>(currentMedia)) {
        stackedWidget->setCurrentIndex(1); // Film is at index 1
        filmDirectorEdit->setText(film->getDirector());
        filmDurationSpinBox->setValue(film->getDuration());
        filmGenreEdit->setText(film->getGenre());
    } else if (MagazineArticle* article = dynamic_cast<MagazineArticle*>(currentMedia)) {
        stackedWidget->setCurrentIndex(2); // MagazineArticle is at index 2
        articleAuthorEdit->setText(article->getAuthor());
        articleMagazineEdit->setText(article->getMagazine());
        articleDoiEdit->setText(article->getDoi());
    }
}

void EditMediaDialog::selectCoverImage() {
    QString imagePath = QFileDialog::getOpenFileName(this, "Seleziona Immagine di Copertina",
                                                     "./biblioteca_virtuale/resources/images/",
                                                     "Immagini (*.png *.jpg *.jpeg *.gif)");
    if (!imagePath.isEmpty()) {
        selectedCoverImagePath = imagePath;
        coverImagePathEdit->setText(QFileInfo(imagePath).fileName());
        QPixmap pixmap(imagePath);
        coverImagePreview->setPixmap(pixmap.scaled(coverImagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void EditMediaDialog::editMediaConfirmed() {
    QString title = titleEdit->text();
    int year = yearSpinBox->value();
    QString coverImagePath = selectedCoverImagePath;

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Errore", "Il titolo non può essere vuoto.");
        return;
    }

    // Update common fields
    currentMedia->setTitle(title);
    currentMedia->setYear(year);
    currentMedia->setCoverImagePath(coverImagePath);

    // Update type-specific fields
    if (Book* book = dynamic_cast<Book*>(currentMedia)) {
        book->setAuthor(bookAuthorEdit->text());
        book->setIsbn(bookIsbnEdit->text());
        book->setPublisher(bookPublisherEdit->text());
    } else if (Film* film = dynamic_cast<Film*>(currentMedia)) {
        film->setDirector(filmDirectorEdit->text());
        film->setDuration(filmDurationSpinBox->value());
        film->setGenre(filmGenreEdit->text());
    } else if (MagazineArticle* article = dynamic_cast<MagazineArticle*>(currentMedia)) {
        article->setAuthor(articleAuthorEdit->text());
        article->setMagazine(articleMagazineEdit->text());
        article->setDoi(articleDoiEdit->text());
    }

    QMessageBox::information(this, "Successo", "Media modificato con successo!");
    accept();
}

