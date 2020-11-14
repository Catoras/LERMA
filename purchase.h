#ifndef PURCHASE_H
#define PURCHASE_H
#include <vector>
#include "product_from_purchase.h"
class Purchase
{
private:
    QString timestamp;
    vector<Product_From_Purchase> chart;
public:
    Purchase();
    QString getTimestamp() const;
    void setTimestamp(const QString &value);
    vector<Product_From_Purchase> getChart() const;
    void setChart(const vector<Product_From_Purchase> &value);

    void addproduct(Product_From_Purchase product);
};

#endif // PURCHASE_H
