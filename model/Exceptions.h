#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <QString>

class BibliotecaException : public std::exception
{
protected:
    std::string message;

public:
    BibliotecaException(const std::string &msg) : message(msg) {}
    virtual const char *what() const noexcept override { return message.c_str(); }
};

class MediaNotFoundException : public BibliotecaException
{
public:
    MediaNotFoundException() : BibliotecaException("Media non trovato") {}
    MediaNotFoundException(const std::string &details)
        : BibliotecaException("Media non trovato: " + details) {}
};

class InvalidDataException : public BibliotecaException
{
public:
    InvalidDataException(const std::string &field)
        : BibliotecaException("Dato non valido: " + field) {}
};

class FileNotFoundException : public BibliotecaException
{
public:
    FileNotFoundException(const std::string &filename)
        : BibliotecaException("File non trovato: " + filename) {}
};

class JsonParseException : public BibliotecaException
{
public:
    JsonParseException(const std::string &details)
        : BibliotecaException("Errore parsing JSON: " + details) {}
};

class DuplicateMediaException : public BibliotecaException
{
public:
    DuplicateMediaException() : BibliotecaException("Media già presente nella biblioteca") {}
};

#endif // EXCEPTIONS_H
