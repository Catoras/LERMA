#include "product_from_purchase.h"

QString Product_From_Purchase::getId() const
{
    return id;
}

void Product_From_Purchase::setId(const QString &value)
{
    id = value;
}

int Product_From_Purchase::getUnits() const
{
    return units;
}

void Product_From_Purchase::setUnits(int value)
{
    units = value;
}
Product_From_Purchase::Product_From_Purchase()
{

}

Product_From_Purchase::Product_From_Purchase(QString id, int units)
{
    this->id=id;
    this->units=units;
}
