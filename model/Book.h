#ifndef BOOK_H
#define BOOK_H

#include "Media.h"
#include "MediaVisitor.h"

class Book : public Media
{
public:
    Book(const QString &title, int year, const QString &author, const QString &isbn, const QString &publisher, const QString &coverImagePath = "");

    // Costruttore di copia
    Book(const Book &other);

    QString getAuthor() const;
    QString getIsbn() const;
    QString getPublisher() const;
    void setAuthor(const QString &author);
    void setIsbn(const QString &isbn);
    void setPublisher(const QString &publisher);

    QString visualizzaDettagli() const override;
    QJsonObject serializza() const override;
    Media *clone() const override;
    static Book *deserializza(const QJsonObject &jsonObject);

    QWidget *accept(MediaVisitor &visitor) override;

private:
    QString author;
    QString isbn;
    QString publisher;
};

#endif // BOOK_H
