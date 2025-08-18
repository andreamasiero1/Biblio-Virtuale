#ifndef USERAUTHENTICATOR_H
#define USERAUTHENTICATOR_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QCryptographicHash>

#include "User.h"

class UserAuthenticator {
public:
    UserAuthenticator(const QString& usersFilePath = "data/users.json");
    ~UserAuthenticator();

    bool authenticate(const QString& username, const QString& password);
    bool addUser(const QString& username, const QString& password);

private:
    QString usersFilePath;
    QList<User*> users;

    void loadUsers();
    void saveUsers();
    QString hashPassword(const QString& password) const;
};

#endif // USERAUTHENTICATOR_H

