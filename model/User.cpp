
#include "User.h"

User::User(const QString& username, const QString& passwordHash)
    : username(username), passwordHash(passwordHash) {}

QString User::getUsername() const {
    return username;
}

QString User::getPasswordHash() const {
    return passwordHash;
}


