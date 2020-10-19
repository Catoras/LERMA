#include "product.h"

QString Product::getId() const
{
    return id;
}

void Product::setId(const QString &value)
{
    id = value;
}

QString Product::getName() const
{
    return name;
}

void Product::setName(const QString &value)
{
    name = value;
}

float Product::getPrice() const
{
    return price;
}

void Product::setPrice(float value)
{
    price = value;
}

Product::Product()
{

}
