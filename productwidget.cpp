#include "productwidget.h"
#include "ui_productwidget.h"
#include <QPixmap>
#include <QDebug>

productwidget::productwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::productwidget)
{
    ui->setupUi(this);
    QPixmap pic(":/Images/Resources/imgs/AB01.jpg");
    ui->label->setPixmap(pic);
    ui->label_2->setText("Camara Nikon FNAF230989 SUPER ESCALABLE GO PARA LLEVAR");
    ui->label_3->setText("$3000");
    ui->groupBox->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px");

}

productwidget::productwidget(QWidget *parent, QString arg1, QString arg2, float arg3) :
    QWidget(parent),
    ui(new Ui::productwidget)
{
    ui->setupUi(this);
    QPixmap picture(":/Images/Resources/imgs/"+arg1+".jpg");
    ui->label->setPixmap(picture);
    productID=arg1;
    ui->label_2->setText(arg2);
    ui->label_3->setText("$"+QString::number(arg3));
}

productwidget::~productwidget()
{
    delete ui;
}

void productwidget::on_pushButton_clicked()
{
    emit addItem(productID,ui->spinBox->value());
    ui->spinBox->setValue(0);
}

void productwidget::on_spinBox_valueChanged(int arg1)
{
    ui->pushButton->setEnabled(arg1);
}
