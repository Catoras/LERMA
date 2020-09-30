#ifndef USER_H
#define USER_H
#include <QString>

class User
{
private:
    QString username;
    QString email;
    QString password;

public:
    User();
    QString getUsername() const;
    void setUsername(const QString &value);
    QString getEmail() const;
    void setEmail(const QString &value);
    QString getPassword() const;
    void setPassword(const QString &value);
};

#endif // USER_H
