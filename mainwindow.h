#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "productwidget.h"
#include "product.h"

#include <QMainWindow>
#include <QMessageBox>
#include "user.h"
#include <regex>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <string.h>
#include <QObject>

#include <QDebug>

#include <QDateTime>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_usernameLE_textChanged(const QString &arg1);

    void on_passwordLE_textChanged(const QString &arg1);

    void on_newPasswordLE_textChanged(const QString &arg1);

    void on_emailLE_textChanged(const QString &arg1);

    void on_newUserLE_textChanged(const QString &arg1);

    void on_signInPB_clicked();

    void on_loginPB_clicked();

    void openFile();

    void on_comboBoxDepartamentos_activated(const QString &arg1);

    void on_comboBoxOrdenamiento_activated(const QString &arg1);

    void on_lineEditBusqueda_textChanged(const QString &arg1);

    void addToChart(QString productID,int amount);

    void updatemain_user();

private:
    Ui::MainWindow *ui;
    vector<User> users;
    QAction* openFileAction;
    QFile dbFile;
    QJsonArray dbArray;
    QJsonArray dbProductsArray;
    User main_user;
    string departments[6]{"Todos los departamentos",
                          "Alimentos y Bebidas",
                          "Libros",
                          "Electrónicos",
                          "Hogar Y Cocina",
                          "Deporte y Aire Libre"};
    vector<Product> products;
/*
    AB - Alimentos y Bebidas
    L - Libros
    E - Electrónicos
    HC - Hogar Y Cocina
    D - Deporte y Aire Libre
*/
    string sorts[3]{"Ordenar",
                    "Ascendente",
                    "Descendente"};

    Purchase actual_purchase;
    void enableLoginPB();
    void enableSignInPB();
    void validateUser();
    bool is_email_valid(const std::string& email);

    void saveDB();
    void loadDB();
    void productfilter();
    void printitems(vector<Product> &array);
};
#endif // MAINWINDOW_H
