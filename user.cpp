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

QString User::getPassword() const
{
    return password;
}

void User::setPassword(const QString &value)
{
    password = value;
}

vector<Purchase> User::getPurchase() const
{
    return purchase;
}

void User::setPurchase(const vector<Purchase> &value)
{
    purchase = value;
}

void User::addpurchase(Purchase purchase)
{
    this->purchase.push_back(purchase);
}

User::User()
{
    
}
