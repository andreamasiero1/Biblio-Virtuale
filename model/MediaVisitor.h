#ifndef MEDIAVISITOR_H
#define MEDIAVISITOR_H

#include <QWidget>

class Book;
class Film;
class MagazineArticle;

class MediaVisitor {
public:
    virtual ~MediaVisitor() = default;
    virtual QWidget* visit(Book* book) = 0;
    virtual QWidget* visit(Film* film) = 0;
    virtual QWidget* visit(MagazineArticle* article) = 0;
};

#endif // MEDIAVISITOR_H

