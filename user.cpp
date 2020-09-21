#include "user.h"

QString User::getUsername() const
{
    return username;
}

void User::setUsername(const QString &value)
{
    username = value;
}

QString User::getEmail() const
{
    return email;
}

void User::setEmail(const QString &value)
{
    email = value;
}

QString User::getPasssword() const
{
    return passsword;
}

void User::setPasssword(const QString &value)
{
    passsword = value;
}

User::User()
{

}
