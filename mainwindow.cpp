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
    update_main_user();
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
    User user_find;

    it = find_if(users.begin(),users.end(),[&user, &password,&user_find] (User lambdauser) -> bool
    {
        if(lambdauser.getUsername()==user && lambdauser.getPassword() == password){
            user_find=lambdauser;
            return true;
        }
        return false;
    });

    if(it == users.end()){
        message.setText("Invalidate Credentials");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }
    else{
        main_user=user_find;
        message.setText("Welcome to Lerma "+ main_user.getUsername());
        ui->viewSW->setCurrentIndex(1);
        ui->menubar->setVisible(0);
        printitems(products);
        vector<Product> vectorofrecommendations = generaterandomrecommendations(5);
        printrecommendations(vectorofrecommendations);
        message.exec();

    }
}

void MainWindow::printitems(vector<Product> &array)
{
    QWidget *central = new QWidget;
    QGridLayout *layout = new QGridLayout(central);
    ui->scrollAreaProductos->setWidget(central);
    ui->scrollAreaProductos->setWidgetResizable(true);
    for(int i = 0; i< array.size();i++){
       productwidget *product= new productwidget(this,
        array[i].getId(),array[i].getName(),array[i].getPrice());
       connect(product, SIGNAL(addItem(QString,int)),this, SLOT(addToChart(QString,int)));
       connect(product, SIGNAL(updaterecommendations(QString)),this, SLOT(updaterecommendations(QString)));
       layout->addWidget(product,i/4,i%4,Qt::AlignCenter);
    }
}

void MainWindow::printrecommendations(vector<Product> &array)
{
    QWidget *central = new QWidget;
    QGridLayout *layout = new QGridLayout(central);
    ui->scrollAreaRecomendaciones->setWidget(central);
    ui->scrollAreaRecomendaciones->setWidgetResizable(true);
    for(int i = 0; i < array.size();i++){
       productwidget *product= new productwidget(this,
        array[i].getId(),array[i].getName(),array[i].getPrice());
       connect(product, SIGNAL(addItem(QString,int)),this,
               SLOT(addToChart(QString,int)));
       connect(product, SIGNAL(updaterecommendations(QString)),this,
               SLOT(updaterecommendations(QString)));
       layout->addWidget(product,i/5,i%5,Qt::AlignCenter);
    }
}

vector<Product> MainWindow::generaterandomrecommendations(int quantity)
{
    uniform_int_distribution<int> distribution(0,products.size()-1);
    vector <Product> vectorRecommendations;

    for(int i = 0 ; i < quantity ;i++){
        vector<Product>::iterator it;
        Product RandomProduct= products.at(distribution(*QRandomGenerator::global()));
        it = find_if(vectorRecommendations.begin(),vectorRecommendations.end(),
                     [&RandomProduct] (Product producto) -> bool
        {
            if(producto.getId()==RandomProduct.getId()){
                return true;
            }
            return false;
        });
        if(it == vectorRecommendations.end())
            vectorRecommendations.push_back(RandomProduct);
        else
            i--;
    }
    return vectorRecommendations;
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
        QJsonArray arraypurchase= obj["purchase"].toArray();
        for(int j=0; j < arraypurchase.size(); j++){
            Purchase purchase;
            QJsonObject compra = arraypurchase[j].toObject();
            QStringList attributes=compra.keys();
            purchase.setTimestamp(attributes.at(0));
            QJsonArray arrayproduct_from_purchase= compra[attributes.at(0)].toArray();
            vector <string> id_from_all_products_from_purchase;
            for(int z=0; z < arrayproduct_from_purchase.size(); z++){
                Product_From_Purchase product_from_purchase;
                QJsonObject producto_de_la_compra = arrayproduct_from_purchase[z].toObject();
                product_from_purchase.setId(producto_de_la_compra["id"].toString());
                product_from_purchase.setUnits(producto_de_la_compra["units"].toInt());
                purchase.addproduct(product_from_purchase);
                id_from_all_products_from_purchase.push_back(product_from_purchase.getId().toStdString());

            }
            user.addpurchase(purchase);
            shopping_network.createEdgesFromAVector(id_from_all_products_from_purchase);
        }
        users.push_back(user);
    }
    shopping_network.printData();

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
    Q_UNUSED(arg1);
    productfilter();
}

void MainWindow::on_comboBoxOrdenamiento_activated(const QString &arg1)
{
    Q_UNUSED(arg1);
    productfilter();
}

void MainWindow::on_lineEditBusqueda_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    productfilter();
}

void MainWindow::addToChart(QString productID, int amount)
{
    Product_From_Purchase producttoadd = Product_From_Purchase(productID,amount);
    actual_purchase.addproduct(producttoadd);
    QDateTime timestamp=QDateTime().currentDateTimeUtc();
    actual_purchase.setTimestamp(timestamp.toString());
}

void MainWindow::update_main_user()
{
    for(int i=0; i < dbArray.size(); i++){
        QJsonObject obj = dbArray[i].toObject();
        if(main_user.getEmail()==obj["email"].toString() and
                main_user.getUsername()==obj["name"].toString()){
            dbArray.removeAt(i);
            break;
        }
    }
    if(actual_purchase.getChart().size())
        main_user.addpurchase(actual_purchase);
    QJsonObject jsonObj;
    QJsonArray jsonarray;
    jsonObj["name"]=main_user.getUsername();
    jsonObj["email"]=main_user.getEmail();
    jsonObj["password"]=main_user.getPassword();
    for(int i=0; i < main_user.getPurchase().size();i++){
        QJsonObject jsonObj2;
        QJsonArray jsonarray2;
        for(int j=0; j < main_user.getPurchase()[i].getChart().size();j++){
           QJsonObject jsonObj3;
           jsonObj3["id"]=main_user.getPurchase()[i].getChart()[j].getId();
           jsonObj3["units"]=main_user.getPurchase()[i].getChart()[j].getUnits();
           jsonarray2.append(jsonObj3);
        }
        jsonObj2[ main_user.getPurchase()[i].getTimestamp()]= jsonarray2;
        jsonarray.append(jsonObj2);
    }
    jsonObj["purchase"]=jsonarray;
    dbArray.append(jsonObj);
}
//https://stackoverflow.com/questions/4485203/c-stl-using-map-with-priority-queue
void MainWindow::updaterecommendations(QString productID)
{
    vector<Product> better_recommendations;
    priority_queue<PAIR,vector<PAIR>,cmp> temp;
    vector<Product> random_recommendations;
    unordered_set<string> filtervectorwithoutduplicades;
    vector<string> vectorproductid;

    if(shopping_network.contains(productID.toStdString())){
        map<string,int> Neighbors = shopping_network.getNeighbors(productID.toStdString());
        map<string,int>::iterator it = Neighbors.begin();
        while(it != Neighbors.end()){
            PAIR par(it->first,it->second);
            recommendations.push(par);
            it++;
        }
    }
    temp=recommendations;
    qDebug() << temp.size();
    qDebug() << recommendations.size();
    for(int i=0;i<temp.size();i++){
        if(filtervectorwithoutduplicades.size()==5)
            break;
        filtervectorwithoutduplicades.insert(temp.top().first);
        temp.pop();
    }
    qDebug() << filtervectorwithoutduplicades.size();
    vectorproductid.assign(filtervectorwithoutduplicades.begin(), filtervectorwithoutduplicades.end());
    qDebug() << vectorproductid.size();
    for(int i =0;i < vectorproductid.size();i++){
        string idfromrecommendations = vectorproductid[i];
        copy_if(products.begin(), products.end(), std::back_inserter(better_recommendations),
                [idfromrecommendations](const Product& c)
        { return c.getId().toStdString() == idfromrecommendations; });
    }
    qDebug() << better_recommendations.size();
    random_recommendations= generaterandomrecommendations(5-better_recommendations.size());
    for(int i = 0; i < random_recommendations.size() ; i++){
        better_recommendations.push_back(random_recommendations[i]);
    }

    printrecommendations(better_recommendations);
}
