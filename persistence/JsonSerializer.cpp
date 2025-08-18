#include "JsonSerializer.h"
#include <QFile>
#include <QJsonParseError>
#include <QDebug>

bool JsonSerializer::salvaBiblioteca(const Biblioteca &biblioteca, const QString &filePath)
{
    try
    {
        salvaBibliotecaThrows(biblioteca, filePath);
        return true;
    }
    catch (const BibliotecaException &)
    {
        return false;
    }
}

bool JsonSerializer::caricaBiblioteca(Biblioteca &biblioteca, const QString &filePath)
{
    try
    {
        caricaBibliotecaThrows(biblioteca, filePath);
        return true;
    }
    catch (const BibliotecaException &)
    {
        return false;
    }
}

void JsonSerializer::salvaBibliotecaThrows(const Biblioteca &biblioteca, const QString &filePath)
{
    QJsonObject jsonObject;
    jsonObject["biblioteca"] = serializeMediaList(biblioteca.getTuttiMedia());

    QJsonDocument jsonDoc(jsonObject);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw FileNotFoundException("Impossibile aprire file per scrittura: " + filePath.toStdString());
    }

    qint64 bytesWritten = file.write(jsonDoc.toJson());
    if (bytesWritten == -1)
    {
        throw BibliotecaException("Errore durante la scrittura del file: " + filePath.toStdString());
    }
    file.close();
}

void JsonSerializer::caricaBibliotecaThrows(Biblioteca &biblioteca, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileNotFoundException(filePath.toStdString());
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw JsonParseException(parseError.errorString().toStdString());
    }

    QJsonObject jsonObject = jsonDoc.object();
    if (!jsonObject.contains("biblioteca"))
    {
        throw JsonParseException("Campo 'biblioteca' mancante nel JSON");
    }

    QJsonArray mediaArray = jsonObject["biblioteca"].toArray();

    biblioteca.svuota(); // Pulisce la biblioteca esistente

    QList<Media *> mediaList = deserializeMediaList(mediaArray);
    for (Media *media : mediaList)
    {
        biblioteca.aggiungiMedia(media);
    }
}

QJsonArray JsonSerializer::serializeMediaList(const QList<Media *> &mediaList)
{
    QJsonArray jsonArray;
    for (const Media *media : mediaList)
    {
        if (media)
        {
            jsonArray.append(media->serializza());
        }
    }
    return jsonArray;
}

QList<Media *> JsonSerializer::deserializeMediaList(const QJsonArray &jsonArray)
{
    QList<Media *> mediaList;
    for (const QJsonValue &value : jsonArray)
    {
        if (value.isObject())
        {
            try
            {
                Media *media = MediaFactory::createFromJson(value.toObject());
                if (media)
                {
                    mediaList.append(media);
                }
            }
            catch (const BibliotecaException &e)
            {
                qDebug() << "Errore nella deserializzazione di un media:" << e.what();
                // Continua con il prossimo media invece di fallire completamente
            }
        }
    }
    return mediaList;
}
