#ifndef CONTAINER_H
#define CONTAINER_H

#include <QList>
#include <algorithm>
#include "Exceptions.h"

template <typename T>
class Container
{
private:
    QList<T *> items;

public:
    Container() = default;

    // Costruttore di copia
    Container(const Container &other)
    {
        for (T *item : other.items)
        {
            if (item)
            {
                // Usa il metodo clone() per creare copie virtuali
                items.append(static_cast<T *>(item->clone()));
            }
        }
    }

    // Operatore di assegnazione
    Container &operator=(const Container &other)
    {
        if (this != &other)
        {
            clear();
            for (T *item : other.items)
            {
                if (item)
                {
                    items.append(static_cast<T *>(item->clone()));
                }
            }
        }
        return *this;
    } // Distruttore
    ~Container()
    {
        clear();
    }

    void add(T *item)
    {
        if (!item)
        {
            throw InvalidDataException("Tentativo di aggiungere un puntatore nullo");
        }
        if (contains(item))
        {
            throw DuplicateMediaException();
        }
        items.append(item);
    }

    void remove(T *item)
    {
        if (!item)
        {
            throw InvalidDataException("Tentativo di rimuovere un puntatore nullo");
        }
        int index = items.indexOf(item);
        if (index == -1)
        {
            throw MediaNotFoundException("Item non presente nel container");
        }
        delete items.takeAt(index);
    }

    void removeAt(int index)
    {
        if (index < 0 || index >= items.size())
        {
            throw MediaNotFoundException("Indice non valido: " + std::to_string(index));
        }
        delete items.takeAt(index);
    }

    T *at(int index) const
    {
        if (index < 0 || index >= items.size())
        {
            throw MediaNotFoundException("Indice non valido: " + std::to_string(index));
        }
        return items.at(index);
    }

    int size() const
    {
        return items.size();
    }

    bool isEmpty() const
    {
        return items.isEmpty();
    }

    bool contains(T *item) const
    {
        return items.contains(item);
    }

    QList<T *> getAll() const
    {
        return items;
    }

    void clear()
    {
        qDeleteAll(items);
        items.clear();
    }

    // Iteratori
    typename QList<T *>::iterator begin()
    {
        return items.begin();
    }

    typename QList<T *>::iterator end()
    {
        return items.end();
    }

    typename QList<T *>::const_iterator begin() const
    {
        return items.begin();
    }

    typename QList<T *>::const_iterator end() const
    {
        return items.end();
    }

    // Operatore di accesso con []
    T *operator[](int index) const
    {
        return at(index);
    }

    // Ricerca
    template <typename Predicate>
    QList<T *> find(Predicate pred) const
    {
        QList<T *> result;
        for (T *item : items)
        {
            if (item && pred(item))
            {
                result.append(item);
            }
        }
        return result;
    }
};

#endif // CONTAINER_H
