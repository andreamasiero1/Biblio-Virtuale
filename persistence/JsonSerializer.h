#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "../model/Biblioteca.h"
#include "../model/Exceptions.h"
#include "../model/MediaFactory.h"

class JsonSerializer
{
public:
    static bool salvaBiblioteca(const Biblioteca &biblioteca, const QString &filePath);
    static bool caricaBiblioteca(Biblioteca &biblioteca, const QString &filePath);

    // Versioni con gestione eccezioni
    static void salvaBibliotecaThrows(const Biblioteca &biblioteca, const QString &filePath);
    static void caricaBibliotecaThrows(Biblioteca &biblioteca, const QString &filePath);

private:
    static QJsonArray serializeMediaList(const QList<Media *> &mediaList);
    static QList<Media *> deserializeMediaList(const QJsonArray &jsonArray);
};

#endif // JSONSERIALIZER_H
