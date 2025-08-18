
#include "UserAuthenticator.h"
#include <QDebug>

UserAuthenticator::UserAuthenticator(const QString& usersFilePath)
    : usersFilePath(usersFilePath) {
    loadUsers();
}

UserAuthenticator::~UserAuthenticator() {
    qDeleteAll(users);
}

bool UserAuthenticator::authenticate(const QString& username, const QString& password) {
    QString hashedPassword = hashPassword(password);
    for (User* user : users) {
        if (user->getUsername() == username && user->getPasswordHash() == hashedPassword) {
            return true;
        }
    }
    return false;
}

bool UserAuthenticator::addUser(const QString& username, const QString& password) {
    // Check if user already exists
    for (User* user : users) {
        if (user->getUsername() == username) {
            qDebug() << "Utente" << username << "esiste già.";
            return false;
        }
    }

    QString hashedPassword = hashPassword(password);
    users.append(new User(username, hashedPassword));
    saveUsers();
    qDebug() << "Utente" << username << "aggiunto con successo.";
    return true;
}

void UserAuthenticator::loadUsers() {
    QFile file(usersFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File utenti non trovato, ne creo uno nuovo:" << usersFilePath;
        // Add default admin user if file doesn't exist
        addUser("admin", "admin");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isArray()) {
        qDebug() << "Errore nel parsing del file utenti JSON.";
        return;
    }

    QJsonArray userArray = doc.array();
    for (const QJsonValue& value : userArray) {
        QJsonObject obj = value.toObject();
        users.append(new User(obj["username"].toString(), obj["passwordHash"].toString()));
    }
}

void UserAuthenticator::saveUsers() {
    QJsonArray userArray;
    for (User* user : users) {
        QJsonObject obj;
        obj["username"] = user->getUsername();
        obj["passwordHash"] = user->getPasswordHash();
        userArray.append(obj);
    }

    QJsonDocument doc(userArray);
    QFile file(usersFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Errore nel salvataggio del file utenti:" << usersFilePath;
        return;
    }
    file.write(doc.toJson());
    file.close();
}

QString UserAuthenticator::hashPassword(const QString& password) const {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}


