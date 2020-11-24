#include "purchase.h"

QString Purchase::getTimestamp() const
{
    return timestamp;
}

void Purchase::setTimestamp(const QString &value)
{
    timestamp = value;
}

vector<Product_From_Purchase> Purchase::getChart() const
{
    return chart;
}

void Purchase::setChart(const vector<Product_From_Purchase> &value)
{
    chart = value;
}

void Purchase::addproduct(Product_From_Purchase product)
{
    chart.push_back(product);
}

Purchase::Purchase()
{
}
