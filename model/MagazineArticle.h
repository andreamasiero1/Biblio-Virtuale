#ifndef MAGAZINEARTICLE_H
#define MAGAZINEARTICLE_H

#include "Media.h"
#include "MediaVisitor.h"

class MagazineArticle : public Media
{
public:
    MagazineArticle(const QString &title, int year, const QString &author, const QString &magazine, const QString &doi, const QString &coverImagePath = "");

    // Costruttore di copia
    MagazineArticle(const MagazineArticle &other);

    QString getAuthor() const;
    QString getMagazine() const;
    QString getDoi() const;
    void setAuthor(const QString &author);
    void setMagazine(const QString &magazine);
    void setDoi(const QString &doi);

    QString visualizzaDettagli() const override;
    QJsonObject serializza() const override;
    Media *clone() const override;
    static MagazineArticle *deserializza(const QJsonObject &jsonObject);

    QWidget *accept(MediaVisitor &visitor) override;

private:
    QString author;
    QString magazine;
    QString doi;
};

#endif // MAGAZINEARTICLE_H
