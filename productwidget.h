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

signals:
    void addItem(QString id,int amount);

private slots:
    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::productwidget *ui;
    QString productID;
};

#endif // PRODUCTWIDGET_H
