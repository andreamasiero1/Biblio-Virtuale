#include "MediaFactory.h"
#include <QStringList>

Media *MediaFactory::createBook(const QString &title, int year, const QString &author,
                                const QString &isbn, const QString &publisher,
                                const QString &coverImagePath)
{
    validateBookData(title, year, author, isbn, publisher);
    return new Book(title, year, author, isbn, publisher, coverImagePath);
}

Media *MediaFactory::createFilm(const QString &title, int year, const QString &director,
                                int duration, const QString &genre,
                                const QString &coverImagePath)
{
    validateFilmData(title, year, director, duration, genre);
    return new Film(title, year, director, duration, genre, coverImagePath);
}

Media *MediaFactory::createMagazineArticle(const QString &title, int year, const QString &author,
                                           const QString &magazine, const QString &doi,
                                           const QString &coverImagePath)
{
    validateMagazineData(title, year, author, magazine, doi);
    return new MagazineArticle(title, year, author, magazine, doi, coverImagePath);
}

Media *MediaFactory::createFromJson(const QJsonObject &jsonObject)
{
    if (!jsonObject.contains("type"))
    {
        throw JsonParseException("Tipo media mancante nel JSON");
    }

    QString type = jsonObject["type"].toString();

    if (type == "Book")
    {
        return Book::deserializza(jsonObject);
    }
    else if (type == "Film")
    {
        return Film::deserializza(jsonObject);
    }
    else if (type == "MagazineArticle")
    {
        return MagazineArticle::deserializza(jsonObject);
    }
    else
    {
        throw InvalidDataException("Tipo media non supportato: " + type.toStdString());
    }
}

Media *MediaFactory::createMedia(const QString &type, const QString &title, int year)
{
    validateCommonData(title, year);

    if (type == "Book")
    {
        return new Book(title, year, "", "", "");
    }
    else if (type == "Film")
    {
        return new Film(title, year, "", 0, "");
    }
    else if (type == "MagazineArticle")
    {
        return new MagazineArticle(title, year, "", "", "");
    }
    else
    {
        throw InvalidDataException("Tipo media non supportato: " + type.toStdString());
    }
}

bool MediaFactory::isValidType(const QString &type)
{
    return getSupportedTypes().contains(type);
}

QStringList MediaFactory::getSupportedTypes()
{
    return QStringList() << "Book" << "Film" << "MagazineArticle";
}

void MediaFactory::validateBookData(const QString &title, int year, const QString &author,
                                    const QString &isbn, const QString &publisher)
{
    validateCommonData(title, year);

    if (author.trimmed().isEmpty())
    {
        throw InvalidDataException("Autore del libro non può essere vuoto");
    }
    if (isbn.trimmed().isEmpty())
    {
        throw InvalidDataException("ISBN non può essere vuoto");
    }
    if (publisher.trimmed().isEmpty())
    {
        throw InvalidDataException("Editore non può essere vuoto");
    }
}

void MediaFactory::validateFilmData(const QString &title, int year, const QString &director,
                                    int duration, const QString &genre)
{
    validateCommonData(title, year);

    if (director.trimmed().isEmpty())
    {
        throw InvalidDataException("Regista non può essere vuoto");
    }
    if (duration <= 0)
    {
        throw InvalidDataException("Durata deve essere maggiore di 0");
    }
    if (genre.trimmed().isEmpty())
    {
        throw InvalidDataException("Genere non può essere vuoto");
    }
}

void MediaFactory::validateMagazineData(const QString &title, int year, const QString &author,
                                        const QString &magazine, const QString &doi)
{
    validateCommonData(title, year);

    if (author.trimmed().isEmpty())
    {
        throw InvalidDataException("Autore dell'articolo non può essere vuoto");
    }
    if (magazine.trimmed().isEmpty())
    {
        throw InvalidDataException("Nome della rivista non può essere vuoto");
    }
    if (doi.trimmed().isEmpty())
    {
        throw InvalidDataException("DOI non può essere vuoto");
    }
}

void MediaFactory::validateCommonData(const QString &title, int year)
{
    if (title.trimmed().isEmpty())
    {
        throw InvalidDataException("Titolo non può essere vuoto");
    }
    if (year < 1000 || year > 2025)
    {
        throw InvalidDataException("Anno deve essere compreso tra 1000 e 2025");
    }
}
