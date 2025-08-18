#ifndef ADDMEDIADIALOG_H
#define ADDMEDIADIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QFormLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "../model/Biblioteca.h"
#include "../model/MediaFactory.h"
#include "../model/Exceptions.h"

class AddMediaDialog : public QDialog
{
    Q_OBJECT

public:
    AddMediaDialog(Biblioteca &biblioteca, QWidget *parent = nullptr);
    ~AddMediaDialog();

private slots:
    void onMediaTypeChanged(int index);
    void selectCoverImage();
    void addMediaConfirmed();

private:
    Biblioteca &biblioteca;
    QStackedWidget *stackedWidget;
    QLineEdit *titleEdit;
    QSpinBox *yearSpinBox;
    QLineEdit *coverImagePathEdit;
    QLabel *coverImagePreview;
    QString selectedCoverImagePath;

    // Book fields
    QLineEdit *bookAuthorEdit;
    QLineEdit *bookIsbnEdit;
    QLineEdit *bookPublisherEdit;

    // Film fields
    QLineEdit *filmDirectorEdit;
    QSpinBox *filmDurationSpinBox;
    QLineEdit *filmGenreEdit;

    // Magazine Article fields
    QLineEdit *articleAuthorEdit;
    QLineEdit *articleMagazineEdit;
    QLineEdit *articleDoiEdit;

    void setupUI();
    void createBookForm();
    void createFilmForm();
    void createMagazineArticleForm();
};

#endif // ADDMEDIADIALOG_H
