#ifndef MEDIAWIDGETVISITOR_H
#define MEDIAWIDGETVISITOR_H

#include "../model/MediaVisitor.h"
#include "../model/Media.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFileInfo>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>

class MediaWidgetVisitor : public QObject, public MediaVisitor
{
    Q_OBJECT

public:
    MediaWidgetVisitor(bool isEditMode = false);
    ~MediaWidgetVisitor();

    QWidget *visit(Book *book) override;
    QWidget *visit(Film *film) override;
    QWidget *visit(MagazineArticle *article) override;

    // Metodo per ottenere il widget creato dopo la visita
    QWidget *getWidget() const;

    // Metodi per ottenere i dati inseriti (solo in modalità edit)
    QString getTitle() const;
    int getYear() const;
    QString getCoverImagePath() const;

    // Metodi specifici per Book
    QString getAuthor() const;
    QString getIsbn() const;
    QString getPublisher() const;

    // Metodi specifici per Film
    QString getDirector() const;
    int getDuration() const;
    QString getGenre() const;

    // Metodi specifici per MagazineArticle
    QString getArticleAuthor() const;
    QString getMagazine() const;
    QString getDoi() const;

    // Metodo per popolare i campi in modalità edit
    void populateFields(Media *media);

private slots:
    void selectCoverImage();

private:
    bool editMode;
    QWidget *currentWidget; // Widget creato durante la visita

    // Campi per modalità edit
    QLineEdit *titleEdit;
    QSpinBox *yearSpin;
    QLineEdit *coverImageEdit;
    QPushButton *browseImageBtn;

    // Campi specifici Book
    QLineEdit *authorEdit;
    QLineEdit *isbnEdit;
    QLineEdit *publisherEdit;

    // Campi specifici Film
    QLineEdit *directorEdit;
    QSpinBox *durationSpin;
    QLineEdit *genreEdit;

    // Campi specifici MagazineArticle
    QLineEdit *articleAuthorEdit;
    QLineEdit *magazineEdit;
    QLineEdit *doiEdit;

    // Metodi helper per modalità display
    QWidget *createBaseWidget(const QString &title, int year, const QString &coverImagePath);
    void addBookSpecificInfo(QWidget *widget, const QString &author, const QString &isbn, const QString &publisher);
    void addFilmSpecificInfo(QWidget *widget, const QString &director, int duration, const QString &genre);
    void addMagazineArticleSpecificInfo(QWidget *widget, const QString &author, const QString &magazine, const QString &doi);

    // Metodi helper per modalità edit
    void createCommonFields(QFormLayout *layout);
    void setupImageSelection();
};

#endif // MEDIAWIDGETVISITOR_H
