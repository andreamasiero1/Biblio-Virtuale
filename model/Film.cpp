#include "Film.h"
#include "MediaVisitor.h"

Film::Film(const QString &title, int year, const QString &director, int duration, const QString &genre, const QString &coverImagePath)
    : Media(title, year, coverImagePath), director(director), duration(duration), genre(genre)
{
}

// Costruttore di copia
Film::Film(const Film &other)
    : Media(other), director(other.director), duration(other.duration), genre(other.genre)
{
}

QString Film::getDirector() const
{
    return director;
}

int Film::getDuration() const
{
    return duration;
}

QString Film::getGenre() const
{
    return genre;
}

void Film::setDirector(const QString &newDirector)
{
    director = newDirector;
}

void Film::setDuration(int newDuration)
{
    duration = newDuration;
}

void Film::setGenre(const QString &newGenre)
{
    genre = newGenre;
}

QString Film::visualizzaDettagli() const
{
    return QString("Film: %1 (%2)\nRegista: %3\nDurata: %4 min\nGenere: %5\nCopertina: %6")
        .arg(title)
        .arg(year)
        .arg(director)
        .arg(duration)
        .arg(genre)
        .arg(coverImagePath);
}

QJsonObject Film::serializza() const
{
    QJsonObject jsonObject;
    jsonObject["type"] = "Film";
    jsonObject["title"] = title;
    jsonObject["year"] = year;
    jsonObject["director"] = director;
    jsonObject["duration"] = duration;
    jsonObject["genre"] = genre;
    jsonObject["coverImagePath"] = coverImagePath;
    return jsonObject;
}

Media *Film::clone() const
{
    return new Film(*this);
}

Film *Film::deserializza(const QJsonObject &jsonObject)
{
    QString title = jsonObject["title"].toString();
    int year = jsonObject["year"].toInt();
    QString director = jsonObject["director"].toString();
    int duration = jsonObject["duration"].toInt();
    QString genre = jsonObject["genre"].toString();
    QString coverImagePath = jsonObject["coverImagePath"].toString();
    return new Film(title, year, director, duration, genre, coverImagePath);
}

QWidget *Film::accept(MediaVisitor &visitor)
{
    return visitor.visit(this);
}
