#include <iostream>
#include <cassert>
#include "../model/Book.h"
#include "../model/Film.h"
#include "../model/MagazineArticle.h"
#include "../model/Biblioteca.h"
#include "../persistence/JsonSerializer.h"

void testBookCreation() {
    Book book("Test Book", 2023, "Test Author", "123-456-789", "Test Publisher");
    assert(book.getTitle() == "Test Book");
    assert(book.getYear() == 2023);
    assert(book.getAuthor() == "Test Author");
    assert(book.getIsbn() == "123-456-789");
    assert(book.getPublisher() == "Test Publisher");
    std::cout << "✓ Test Book Creation passed" << std::endl;
}

void testFilmCreation() {
    Film film("Test Film", 2023, "Test Director", 120, "Action");
    assert(film.getTitle() == "Test Film");
    assert(film.getYear() == 2023);
    assert(film.getDirector() == "Test Director");
    assert(film.getDuration() == 120);
    assert(film.getGenre() == "Action");
    std::cout << "✓ Test Film Creation passed" << std::endl;
}

void testMagazineArticleCreation() {
    MagazineArticle article("Test Article", 2023, "Test Author", "Test Magazine", "10.1000/test");
    assert(article.getTitle() == "Test Article");
    assert(article.getYear() == 2023);
    assert(article.getAuthor() == "Test Author");
    assert(article.getMagazine() == "Test Magazine");
    assert(article.getDoi() == "10.1000/test");
    std::cout << "✓ Test Magazine Article Creation passed" << std::endl;
}

void testBibliotecaOperations() {
    Biblioteca biblioteca;
    
    Book* book = new Book("Test Book", 2023, "Test Author", "123-456-789", "Test Publisher");
    Film* film = new Film("Test Film", 2023, "Test Director", 120, "Action");
    
    biblioteca.aggiungiMedia(book);
    biblioteca.aggiungiMedia(film);
    
    assert(biblioteca.dimensione() == 2);
    assert(biblioteca.getLibri().size() == 1);
    assert(biblioteca.getFilm().size() == 1);
    
    auto risultatiRicerca = biblioteca.cercaPerTitolo("Test");
    assert(risultatiRicerca.size() == 2);
    
    std::cout << "✓ Test Biblioteca Operations passed" << std::endl;
}

void testSerialization() {
    Book book("Test Book", 2023, "Test Author", "123-456-789", "Test Publisher");
    QJsonObject jsonObj = book.serializza();
    
    assert(jsonObj["type"].toString() == "Book");
    assert(jsonObj["title"].toString() == "Test Book");
    assert(jsonObj["year"].toInt() == 2023);
    
    Book* deserializedBook = Book::deserializza(jsonObj);
    assert(deserializedBook->getTitle() == "Test Book");
    assert(deserializedBook->getYear() == 2023);
    assert(deserializedBook->getAuthor() == "Test Author");
    
    delete deserializedBook;
    std::cout << "✓ Test Serialization passed" << std::endl;
}

int main() {
    std::cout << "Running Model Tests..." << std::endl;
    
    testBookCreation();
    testFilmCreation();
    testMagazineArticleCreation();
    testBibliotecaOperations();
    testSerialization();
    
    std::cout << "All tests passed! ✓" << std::endl;
    return 0;
}

