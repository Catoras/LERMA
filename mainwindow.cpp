#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openFileAction = new QAction("&Open Database", this);
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    ui->menubar->addMenu("&File")->addAction(openFileAction);
}

MainWindow::~MainWindow()
{
    saveDB();
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
            is_email_valid(ui->emailLE->text().toStdString())){
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
        return lambdauser.getUsername()==user && lambdauser.getPassword() == password;
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

bool MainWindow::is_email_valid(const string &email)
{
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}

void MainWindow::saveDB()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;

    jsonObj["users"] = dbArray;
    jsonDoc = QJsonDocument(jsonObj);

    dbFile.open(QIODevice::WriteOnly);
    dbFile.write(jsonDoc.toJson());
    dbFile.close();
}

void MainWindow::loadDB()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray data;

    dbFile.open(QIODevice::ReadOnly);
    data = dbFile.readAll();
    dbFile.close();

    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    dbArray = jsonObj["users"].toArray();

    for(int i=0; i < dbArray.size(); i++){
        User user;
        QJsonObject obj = dbArray[i].toObject();
        user.setUsername(obj["name"].toString());
        user.setEmail(obj["email"].toString());
        user.setPassword(obj["password"].toString());
        users.push_back(user);
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
    QJsonObject jsonObj;
    QMessageBox message;
    User user;
    vector<User>::iterator it;
    QString email;
    QString username;

    user.setUsername(ui->newUserLE->text());
    user.setEmail(ui->emailLE->text());
    user.setPassword(ui->newPasswordLE->text());

    username = user.getUsername();
    email = user.getEmail();

    it = find_if(users.begin(),users.end(),[&username, &email] (User lambdauser) -> bool
    {
        return lambdauser.getUsername()==username || lambdauser.getEmail() == email;
    });

    if(it == users.end()){
        users.push_back(user);

        message.setText("New user created.");
        message.exec();

        jsonObj["name"]=user.getUsername();
        jsonObj["email"]=user.getEmail();
        jsonObj["password"]=user.getPassword();
        dbArray.append(jsonObj);
    }
    else{
        message.setText("Nombre o correo no disponible.");
        message.exec();
    }

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

void MainWindow::openFile()
{
    QString name;
    name = QFileDialog::getOpenFileName(this, "Select Database","","JSON files (*.json)");
    if (name.length()){
        dbFile.setFileName(name);
        ui->loginGB->setEnabled(true);
        ui->signInGB->setEnabled(true);
        loadDB();
    }
}
