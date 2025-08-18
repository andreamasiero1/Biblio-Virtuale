#ifndef FILM_H
#define FILM_H

#include "Media.h"
#include "MediaVisitor.h"

class Film : public Media
{
public:
    Film(const QString &title, int year, const QString &director, int duration, const QString &genre, const QString &coverImagePath = "");

    // Costruttore di copia
    Film(const Film &other);

    QString getDirector() const;
    int getDuration() const;
    QString getGenre() const;
    void setDirector(const QString &director);
    void setDuration(int duration);
    void setGenre(const QString &genre);

    QString visualizzaDettagli() const override;
    QJsonObject serializza() const override;
    Media *clone() const override;
    static Film *deserializza(const QJsonObject &jsonObject);

    QWidget *accept(MediaVisitor &visitor) override;

private:
    QString director;
    int duration;
    QString genre;
};

#endif // FILM_H
