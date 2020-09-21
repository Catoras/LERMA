#ifndef USER_H
#define USER_H
#include <QString>

class User
{
private:
    QString username;
    QString email;
    QString passsword;

public:
    User();
    QString getUsername() const;
    void setUsername(const QString &value);
    QString getEmail() const;
    void setEmail(const QString &value);
    QString getPasssword() const;
    void setPasssword(const QString &value);
};

#endif // USER_H
