#ifndef MEDIACOLLECTORVISITOR_H
#define MEDIACOLLECTORVISITOR_H

#include "../model/MediaVisitor.h"
#include "../model/Biblioteca.h"
#include <QList>
#include <QString>

class Media;
class Book;
class Film;
class MagazineArticle;

/**
 * MediaCollectorVisitor - Pattern Visitor per raccogliere Media per tipo
 *
 * Questo visitor implementa il polimorfismo in maniera non banale:
 * - Ogni metodo visit() ha un comportamento diverso basato sul tipo dinamico
 * - Utilizza il pattern Visitor invece del controllo di tipo esplicito
 * - Dimostra un uso sofisticato del polimorfismo per filtrare collezioni
 */
class MediaCollectorVisitor : public MediaVisitor
{
public:
    // Usa l'enum dal namespace MediaFilter per coerenza
    using FilterType = MediaFilter::FilterType;

private:
    QList<Media *> collectedMedia;
    FilterType filterType;

public:
    /**
     * Costruttore del MediaCollectorVisitor
     * @param type Tipo di filtro da applicare durante la raccolta
     */
    explicit MediaCollectorVisitor(FilterType type = FilterType::ALL);

    /**
     * Visita un oggetto Book
     * Comportamento polimorfo: raccoglie il Book solo se il filtro lo consente
     * @param book Puntatore al Book da visitare
     * @return nullptr (questo visitor non genera widget)
     */
    QWidget *visit(Book *book) override;

    /**
     * Visita un oggetto Film
     * Comportamento polimorfo: raccoglie il Film solo se il filtro lo consente
     * @param film Puntatore al Film da visitare
     * @return nullptr (questo visitor non genera widget)
     */
    QWidget *visit(Film *film) override;

    /**
     * Visita un oggetto MagazineArticle
     * Comportamento polimorfo: raccoglie l'articolo solo se il filtro lo consente
     * @param article Puntatore al MagazineArticle da visitare
     * @return nullptr (questo visitor non genera widget)
     */
    QWidget *visit(MagazineArticle *article) override;

    /**
     * Ottiene i Media raccolti durante la visita
     * @return Lista dei Media che soddisfano il criterio di filtro
     */
    QList<Media *> getCollectedMedia() const;

    /**
     * Pulisce la collezione per riutilizzare il visitor
     */
    void clearCollection();

    /**
     * Cambia il tipo di filtro
     * @param type Nuovo tipo di filtro da applicare
     */
    void setFilterType(FilterType type);
};

#endif // MEDIACOLLECTORVISITOR_H
