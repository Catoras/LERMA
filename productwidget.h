#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>

namespace Ui {
class productwidget;
}

class productwidget : public QWidget
{
    Q_OBJECT

public:
    explicit productwidget(QWidget *parent = nullptr);
    productwidget(QWidget *parent = nullptr,QString arg1 = "",QString arg2 = "",float arg3 = 0);
    ~productwidget();

private:
    Ui::productwidget *ui;
};

#endif // PRODUCTWIDGET_H
