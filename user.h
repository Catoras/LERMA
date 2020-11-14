#ifndef USER_H
#define USER_H
#include "purchase.h"

class User
{
private:
    QString username;
    QString email;
    QString password;
    vector<Purchase> purchase;

public:
    User();
    QString getUsername() const;
    void setUsername(const QString &value);
    QString getEmail() const;
    void setEmail(const QString &value);
    QString getPassword() const;
    void setPassword(const QString &value);
    vector<Purchase> getPurchase() const;
    void setPurchase(const vector<Purchase> &value);

    void addpurchase(Purchase purchase);
};

#endif // USER_H
