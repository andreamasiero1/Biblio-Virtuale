#include "MediaCollectorVisitor.h"
#include "../model/Media.h"
#include "../model/Book.h"
#include "../model/Film.h"
#include "../model/MagazineArticle.h"

/**
 * Costruttore del MediaCollectorVisitor
 * Inizializza il visitor con il tipo di filtro specificato
 */
MediaCollectorVisitor::MediaCollectorVisitor(FilterType type)
    : filterType(type)
{
    // La lista collectedMedia viene inizializzata automaticamente vuota
}

/**
 * Implementazione polimorfa per Book
 * Questo metodo si comporta diversamente in base al filterType:
 * - Se ALL o BOOKS_ONLY: raccoglie il Book
 * - Altrimenti: ignora il Book
 * Dimostra polimorfismo non banale attraverso comportamenti specifici per tipo
 */
QWidget *MediaCollectorVisitor::visit(Book *book)
{
    if (book && (filterType == FilterType::ALL || filterType == FilterType::BOOKS_ONLY))
    {
        collectedMedia.append(book);
    }
    return nullptr; // Questo visitor non genera widget
}

/**
 * Implementazione polimorfa per Film
 * Comportamento specifico per Film: raccoglie solo se il filtro lo consente
 * Dimostra come ogni tipo di Media ha un comportamento visitor diverso
 */
QWidget *MediaCollectorVisitor::visit(Film *film)
{
    if (film && (filterType == FilterType::ALL || filterType == FilterType::FILMS_ONLY))
    {
        collectedMedia.append(film);
    }
    return nullptr;
}

/**
 * Implementazione polimorfa per MagazineArticle
 * Comportamento specifico per articoli: logica di raccolta dedicata
 * Ogni implementazione di visit() può avere logica completamente diversa
 */
QWidget *MediaCollectorVisitor::visit(MagazineArticle *article)
{
    if (article && (filterType == FilterType::ALL || filterType == FilterType::ARTICLES_ONLY))
    {
        collectedMedia.append(article);
    }
    return nullptr;
}

/**
 * Ottiene i Media raccolti durante la visita
 * Fornisce accesso al risultato della raccolta polimorfa
 */
QList<Media *> MediaCollectorVisitor::getCollectedMedia() const
{
    return collectedMedia;
}

/**
 * Pulisce la collezione per riutilizzare il visitor
 * Consente di riutilizzare lo stesso visitor per multiple raccolte
 */
void MediaCollectorVisitor::clearCollection()
{
    collectedMedia.clear();
}

/**
 * Cambia il tipo di filtro dinamicamente
 * Permette di riconfigurare il comportamento del visitor
 */
void MediaCollectorVisitor::setFilterType(FilterType type)
{
    filterType = type;
}
