#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>


class Product
{
private:
    QString id;
    QString name;
    float price;
public:
    Product();
    QString getId() const;
    void setId(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    float getPrice() const;
    void setPrice(float value);
};

#endif // PRODUCT_H
