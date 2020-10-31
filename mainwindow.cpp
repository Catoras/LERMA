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
    //Combobox
    for (string department : departments)
        ui->comboBoxDepartamentos->addItem(QString::fromStdString(department));
    for(string sort : sorts)
        ui->comboBoxOrdenamiento->addItem(QString::fromStdString(sort));
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
        ui->menubar->setVisible(0);
        printitems(products);

        message.exec();

    }
}

void MainWindow::printitems(vector<Product> &array)
{
    QWidget *central = new QWidget;
    QGridLayout *layout = new QGridLayout(central);
    ui->scrollAreaProductos->setWidget(central);
    ui->scrollAreaProductos->setWidgetResizable(true);

    int row=0;
    int column=0;
    for(int i = 0; i< array.size();i++){
       productwidget *product= new productwidget(this,
        array[i].getId(),array[i].getName(),array[i].getPrice());
       layout->addWidget(product,row,column++);
       if(column == 4){
           column=0;
           row++;
       }
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
    jsonObj["products"]= dbProductsArray;


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
    dbProductsArray=jsonObj["products"].toArray();

    for(int i=0; i < dbArray.size(); i++){
        User user;
        QJsonObject obj = dbArray[i].toObject();
        user.setUsername(obj["name"].toString());
        user.setEmail(obj["email"].toString());
        user.setPassword(obj["password"].toString());
        users.push_back(user);
    }

    for(int i=0; i < dbProductsArray.size(); i++){
        Product product;
        QJsonObject obj = dbProductsArray[i].toObject();
        product.setId(obj["id"].toString());
        product.setName(obj["name"].toString());
        product.setPrice(obj["price"].toDouble());
        products.push_back(product);
    }
}

void MainWindow::productfilter()
{
    QString arg1 = ui->comboBoxDepartamentos->currentText();
    QString arg2 = ui->comboBoxOrdenamiento->currentText();
    QString arg3 = ui->lineEditBusqueda->text();
    vector<Product> filteredproducts;

    if(arg1==QString::fromStdString(departments[0])){
        copy_if(products.begin(), products.end(), std::back_inserter(filteredproducts), [arg3](const Product& c)
        { return c.getName().toLower().toStdString().find(arg3.toLower().toStdString()) != string::npos; });
    }
    else if(arg1==QString::fromStdString(departments[1])){
        copy_if(products.begin(), products.end(), std::back_inserter(filteredproducts), [arg3](const Product& c)
        { return c.getId()[0]=="A" && c.getId()[1]=="B" && c.getName().toLower().toStdString().find(arg3.toLower().toStdString()) != string::npos; });
    }
    else if(arg1==QString::fromStdString(departments[2])){
        copy_if(products.begin(), products.end(), std::back_inserter(filteredproducts), [arg3](const Product& c)
        { return c.getId()[0]=="L" && c.getName().toLower().toStdString().find(arg3.toLower().toStdString()) != string::npos; });
    }
    else if(arg1==QString::fromStdString(departments[3])){
        copy_if(products.begin(), products.end(), std::back_inserter(filteredproducts), [arg3](const Product& c)
        { return c.getId()[0]=="E" && c.getName().toLower().toStdString().find(arg3.toLower().toStdString()) != string::npos; });
    }
    else if(arg1==QString::fromStdString(departments[4])){
        copy_if(products.begin(), products.end(), std::back_inserter(filteredproducts), [arg3](const Product& c)
        { return c.getId()[0]=="H" && c.getId()[1]=="C" && c.getName().toLower().toStdString().find(arg3.toLower().toStdString()) != string::npos; });
    }
    else if(arg1==QString::fromStdString(departments[5])){
        copy_if(products.begin(), products.end(), std::back_inserter(filteredproducts), [arg3](const Product& c)
        { return c.getId()[0]=="D" && c.getName().toLower().toStdString().find(arg3.toLower().toStdString()) != string::npos; });
    }
    else{}

    sort(filteredproducts.begin(), filteredproducts.end(),
        [arg2](const Product & a, const Product & b) -> bool
    {
        if(arg2=="Ascendente"){
            return a.getPrice() > b.getPrice();
        }
        else if(arg2=="Descendente"){
            return a.getPrice() < b.getPrice();
        }
        else{}
    });
    printitems(filteredproducts);
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

void MainWindow::on_comboBoxDepartamentos_activated(const QString &arg1)
{
    productfilter();
}

void MainWindow::on_comboBoxOrdenamiento_activated(const QString &arg1)
{
    productfilter();
}

void MainWindow::on_lineEditBusqueda_textChanged(const QString &arg1)
{
    productfilter();
}
