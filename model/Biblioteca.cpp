#include "Biblioteca.h"
#include "Book.h"
#include "Film.h"
#include "MagazineArticle.h"

/**
 * Costruttore di default della Biblioteca.
 * Inizializza una biblioteca vuota con container interno pronto all'uso.
 */
Biblioteca::Biblioteca() {}

/**
 * Distruttore della Biblioteca.
 * Rilascia automaticamente tutte le risorse e libera la memoria
 * dei Media contenuti attraverso il metodo svuota().
 */
Biblioteca::~Biblioteca()
{
    svuota();
}

/**
 * Costruttore di copia della Biblioteca.
 * Crea una nuova biblioteca copiando tutti i Media da un'altra biblioteca.
 * Utilizza il Container template che gestisce automaticamente il deep copy
 * tramite il metodo clone() di ogni Media, garantendo indipendenza tra oggetti.
 * @param other La biblioteca sorgente da copiare
 */
// Costruttore di copia - ora gestito dal Container
Biblioteca::Biblioteca(const Biblioteca &other) : mediaContainer(other.mediaContainer)
{
    // Il Container gestisce automaticamente il deep copy
}

/**
 * Operatore di assegnazione della Biblioteca.
 * Copia il contenuto di un'altra biblioteca in questa istanza.
 * Gestisce correttamente l'auto-assegnazione e utilizza il Container template
 * per il deep copy automatico di tutti i Media contenuti.
 * @param other La biblioteca sorgente da cui copiare
 * @return Riferimento a questa biblioteca modificata
 */
// Operatore di assegnazione - ora gestito dal Container
Biblioteca &Biblioteca::operator=(const Biblioteca &other)
{
    if (this != &other)
    {
        mediaContainer = other.mediaContainer;
    }
    return *this;
}

/**
 * Aggiunge un nuovo Media alla biblioteca.
 * Delega al Container template l'inserimento del Media nel contenitore.
 * Il Container verifica automaticamente duplicati e gestisce eccezioni.
 * @param media Puntatore al Media da aggiungere (ownership trasferita alla biblioteca)
 * @throws DuplicateMediaException se il Media è già presente
 * @throws InvalidDataException se il Media è null o ha dati non validi
 */
void Biblioteca::aggiungiMedia(Media *media)
{
    try
    {
        mediaContainer.add(media);
    }
    catch (const BibliotecaException &e)
    {
        throw; // rilancia l'eccezione per la gestione a livello superiore
    }
}

/**
 * Rimuove un Media specifico dalla biblioteca.
 * Cerca il Media nel container e lo rimuove se presente.
 * La memoria del Media rimosso viene automaticamente liberata dal Container.
 * @param media Puntatore al Media da rimuovere
 * @return true se il Media è stato trovato e rimosso, false altrimenti
 */
bool Biblioteca::rimuoviMedia(Media *media)
{
    try
    {
        mediaContainer.remove(media);
        return true;
    }
    catch (const MediaNotFoundException &)
    {
        return false;
    }
}

/**
 * Rimuove un Media dalla biblioteca tramite il suo indice posizionale.
 * Utilizza l'indice per identificare univocamente il Media da rimuovere.
 * La memoria del Media rimosso viene automaticamente liberata dal Container.
 * @param index Indice posizionale del Media da rimuovere (0-based)
 * @throws std::out_of_range se l'indice è fuori dai limiti validi
 */
void Biblioteca::rimuoviMediaAt(int index)
{
    mediaContainer.removeAt(index);
}

/**
 * Ottiene un Media dalla biblioteca tramite il suo indice posizionale.
 * Fornisce accesso diretto a un Media specifico senza rimuoverlo.
 * @param index Indice posizionale del Media da recuperare (0-based)
 * @return Puntatore al Media alla posizione specificata
 * @throws std::out_of_range se l'indice è fuori dai limiti validi
 */
Media *Biblioteca::getMediaAt(int index) const
{
    return mediaContainer.at(index);
}

/**
 * Ricerca Media per titolo nella biblioteca.
 * Effettua una ricerca case-insensitive che trova tutti i Media
 * il cui titolo contiene la stringa specificata.
 * @param titolo Stringa da cercare nei titoli (ricerca parziale)
 * @return Lista di puntatori ai Media che corrispondono al criterio
 */
QList<Media *> Biblioteca::cercaPerTitolo(const QString &titolo) const
{
    return mediaContainer.find([&titolo](Media *media)
                               { return media->getTitle().contains(titolo, Qt::CaseInsensitive); });
}

/**
 * Ricerca Media per anno di pubblicazione nella biblioteca.
 * Trova tutti i Media pubblicati nell'anno specificato.
 * @param anno Anno di pubblicazione da cercare
 * @return Lista di puntatori ai Media pubblicati nell'anno specificato
 */
QList<Media *> Biblioteca::cercaPerAnno(int anno) const
{
    return mediaContainer.find([anno](Media *media)
                               { return media->getYear() == anno; });
}

/**
 * Ottiene tutti i Media presenti nella biblioteca.
 * Fornisce accesso completo all'intera collezione per operazioni di visualizzazione.
 * @return Lista di puntatori a tutti i Media contenuti nella biblioteca
 */
QList<Media *> Biblioteca::getTuttiMedia() const
{
    return mediaContainer.getAll();
}

/**
 * Ottiene tutti i Libri presenti nella biblioteca.
 * Filtra la collezione utilizzando dynamic_cast per identificare solo gli oggetti Book.
 * @return Lista di puntatori ai soli oggetti Book contenuti nella biblioteca
 */
QList<Media *> Biblioteca::getLibri() const
{
    return mediaContainer.find([](Media *media)
                               { return dynamic_cast<Book *>(media) != nullptr; });
}

/**
 * Ottiene tutti i Film presenti nella biblioteca.
 * Filtra la collezione utilizzando dynamic_cast per identificare solo gli oggetti Film.
 * @return Lista di puntatori ai soli oggetti Film contenuti nella biblioteca
 */
QList<Media *> Biblioteca::getFilm() const
{
    return mediaContainer.find([](Media *media)
                               { return dynamic_cast<Film *>(media) != nullptr; });
}

/**
 * Ottiene tutti gli Articoli di Rivista presenti nella biblioteca.
 * Filtra la collezione utilizzando dynamic_cast per identificare solo gli oggetti MagazineArticle.
 * @return Lista di puntatori ai soli oggetti MagazineArticle contenuti nella biblioteca
 */
QList<Media *> Biblioteca::getArticoli() const
{
    return mediaContainer.find([](Media *media)
                               { return dynamic_cast<MagazineArticle *>(media) != nullptr; });
}

/**
 * Svuota completamente la biblioteca.
 * Rimuove tutti i Media dal container e libera automaticamente la memoria.
 * Delega al Container template la gestione della deallocazione sicura.
 */
void Biblioteca::svuota()
{
    mediaContainer.clear();
}

/**
 * Ottiene il numero di Media presenti nella biblioteca.
 * Fornisce informazioni sulla dimensione attuale della collezione.
 * @return Numero intero di Media contenuti nella biblioteca
 */
int Biblioteca::dimensione() const
{
    return mediaContainer.size();
}

/**
 * Verifica se la biblioteca è vuota.
 * Controlla se non ci sono Media contenuti nella collezione.
 * @return true se la biblioteca non contiene alcun Media, false altrimenti
 */
bool Biblioteca::isEmpty() const
{
    return mediaContainer.isEmpty();
}
