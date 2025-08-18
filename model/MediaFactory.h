#ifndef MEDIAFACTORY_H
#define MEDIAFACTORY_H

#include <QString>
#include <QJsonObject>
#include "Media.h"
#include "Book.h"
#include "Film.h"
#include "MagazineArticle.h"
#include "Exceptions.h"

class MediaFactory
{
public:
    // Crea un Media dal tipo e dai parametri
    static Media *createBook(const QString &title, int year, const QString &author,
                             const QString &isbn, const QString &publisher,
                             const QString &coverImagePath = "");

    static Media *createFilm(const QString &title, int year, const QString &director,
                             int duration, const QString &genre,
                             const QString &coverImagePath = "");

    static Media *createMagazineArticle(const QString &title, int year, const QString &author,
                                        const QString &magazine, const QString &doi,
                                        const QString &coverImagePath = "");

    // Crea un Media da JSON (per deserializzazione)
    static Media *createFromJson(const QJsonObject &jsonObject);

    // Crea un Media da tipo stringa e parametri base
    static Media *createMedia(const QString &type, const QString &title, int year);

    // Validazione tipi supportati
    static bool isValidType(const QString &type);
    static QStringList getSupportedTypes();

private:
    MediaFactory() = default; // Factory non deve essere istanziata

    // Metodi di validazione privati
    static void validateBookData(const QString &title, int year, const QString &author,
                                 const QString &isbn, const QString &publisher);
    static void validateFilmData(const QString &title, int year, const QString &director,
                                 int duration, const QString &genre);
    static void validateMagazineData(const QString &title, int year, const QString &author,
                                     const QString &magazine, const QString &doi);
    static void validateCommonData(const QString &title, int year);
};

#endif // MEDIAFACTORY_H
