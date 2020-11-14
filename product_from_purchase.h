#ifndef PRODUCT_FROM_PURCHASE_H
#define PRODUCT_FROM_PURCHASE_H
#include <QString>
using namespace std;

class Product_From_Purchase
{
private:
    QString id;
    int units;
public:
    Product_From_Purchase();
    Product_From_Purchase(QString id,int units);
    QString getId() const;
    void setId(const QString &value);
    int getUnits() const;
    void setUnits(int value);
};

#endif // PRODUCT_FROM_PURCHASE_H
