#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableLoginPB()
{
    if(ui->usernameLE->text().length() and
            ui->passwordLE->text().length()){
        ui->loginPB->setEnabled(true);
    }
    else{
        ui->loginPB->setEnabled(false);
    }
}

void MainWindow::enableSignInPB()
{
    if(ui->newUserLE->text().length() and
            ui->newPasswordLE->text().length() and
            ui->emailLE->text().length()){

        ui->signInPB->setEnabled(true);
    }
    else{
        ui->signInPB->setEnabled(false);
    }
}

void MainWindow::validateUser()
{
    QMessageBox message;
    vector<User>::iterator it;
    QString user = ui->usernameLE->text();
    QString password =ui->passwordLE->text();

    it = find_if(users.begin(),users.end(),[&user, &password] (User lambdauser) -> bool
    {
        return lambdauser.getUsername()==user && lambdauser.getPasssword() == password;
    });

    if(it == users.end()){
        message.setText("Invalidate Credentials");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }
    else{
        message.setText("Welcome to Lerma "+ user);
        ui->viewSW->setCurrentIndex(1);
        message.exec();
    }
}


void MainWindow::on_usernameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_newUserLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_signInPB_clicked()
{
    QMessageBox message;
    User user;

    user.setUsername(ui->newUserLE->text());
    user.setEmail(ui->emailLE->text());
    user.setPasssword(ui->newPasswordLE->text());

    users.push_back(user);

    message.setText("New user created.");
    message.exec();

    ui->newUserLE->clear();
    ui->emailLE->clear();
    ui->newPasswordLE->clear();
}

void MainWindow::on_loginPB_clicked()
{
    validateUser();

    ui->usernameLE->clear();
    ui->passwordLE->clear();
}
