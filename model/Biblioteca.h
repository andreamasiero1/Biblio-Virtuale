#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <QList>
#include <QString>
#include "Media.h"
#include "Container.h"
#include "Exceptions.h"

class Biblioteca {
public:
    Biblioteca();
    ~Biblioteca();
    
    // Costruttore di copia e operatore di assegnazione per gestione corretta della memoria
    Biblioteca(const Biblioteca& other);
    Biblioteca& operator=(const Biblioteca& other);

    void aggiungiMedia(Media* media);
    bool rimuoviMedia(Media* media);
    void rimuoviMediaAt(int index);
    Media* getMediaAt(int index) const;
    QList<Media*> cercaPerTitolo(const QString& titolo) const;
    QList<Media*> cercaPerAnno(int anno) const;
    QList<Media*> getTuttiMedia() const;
    QList<Media*> getLibri() const;
    QList<Media*> getFilm() const;
    QList<Media*> getArticoli() const;
    
    void svuota();
    int dimensione() const;
    bool isEmpty() const;

private:
    Container<Media> mediaContainer;
};

#endif // BIBLIOTECA_H

