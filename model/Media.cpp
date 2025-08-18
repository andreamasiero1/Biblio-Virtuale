#include "Media.h"
#include "Book.h"
#include "Film.h"
#include "MagazineArticle.h"

Media::Media(const QString& title, int year, const QString& coverImagePath)
    : title(title), year(year), coverImagePath(coverImagePath) {}

QString Media::getTitle() const {
    return title;
}

int Media::getYear() const {
    return year;
}

QString Media::getCoverImagePath() const {
    return coverImagePath;
}

void Media::setCoverImagePath(const QString& path) {
    coverImagePath = path;
}

void Media::setTitle(const QString& newTitle) {
    title = newTitle;
}

void Media::setYear(int newYear) {
    year = newYear;
}

Media* Media::deserializza(const QJsonObject& jsonObject) {
    QString type = jsonObject["type"].toString();
    if (type == "Book") {
        return Book::deserializza(jsonObject);
    } else if (type == "Film") {
        return Film::deserializza(jsonObject);
    } else if (type == "MagazineArticle") {
        return MagazineArticle::deserializza(jsonObject);
    }
    return nullptr; // O lanciare un'eccezione
}

