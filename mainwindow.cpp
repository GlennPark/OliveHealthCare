#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "customermanage.h"
#include "merchandisemanage.h"
#include "purchasemanage.h"

#include "customersearch.h"
#include "merchandisesearch.h"
#include "purchasesearch.h"

#include "chattingserver.h"
#include "chattingclient.h"

#include <QtSql>






MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //DB 셋업
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

//    db.setHostName("OliveHealthCare");
//    db.setDatabaseName("OliveHealthCare");
//    db.setUserName("hr");
//    db.setPassword("hr");

//    bool ok = db.open();

//    QSqlQuery qry;
//    qry.exec("SELECT Cid, name FROM customer");

//    while(qry.next())
//    {

//        int Cid = qry.value(0).toInt();
//        QString name = qry.value(1).toString();
//        qDebug() << Cid << name;
//    }

//    //qry.exec("INSERY INTO customer (Cid, name) VALUES (005, '영선')");
//    //placeholder 방식
//    qry.prepare("INSERY INTO customer (Cid, name) VALUES (:id, :name, :salary)");
//    for(;;)
//    {
//        qry.bindValue(":Cid", 005);
//        qry.bindValue(":name", "영선");
//    }

    customerSearch = new CustomerSearch(this);
    //    ui->mdiArea->addSubWindow(customerSearch);


    merchandiseSearch = new MerchandiseSearch(this);
    //    ui->mdiArea->addSubWindow(merchandiseSearch);

    purchaseSearch = new PurchaseSearch(this);
    //    ui->mdiArea->addSubWindow(purchaseSearch);

    customerManage = new CustomerManage(this);
    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(customerManage);
    customerManage->dataSave();
    customerManage->setWindowTitle(tr("Customer Manager"));
    customerManage->setWindowFlags(Qt::FramelessWindowHint);
    customerManage->showMaximized();

    merchandiseManage = new MerchandiseManage(this);
    ui->mdiArea->addSubWindow(merchandiseManage);
    merchandiseManage->dataSave();
    merchandiseManage->setWindowTitle(tr("Merchandise Manager"));
    merchandiseManage->setWindowFlag(Qt::FramelessWindowHint);
    merchandiseManage->showMaximized();

    purchaseManage = new PurchaseManage(this);
    ui->mdiArea->addSubWindow(purchaseManage);
    purchaseManage->dataSave();
    purchaseManage->setWindowTitle(tr("Purchase Manager"));
    purchaseManage->setWindowFlag(Qt::FramelessWindowHint);
    purchaseManage->showMaximized();

    chattingServer = new ChattingServer(this);
    ui->mdiArea->addSubWindow(chattingServer);
    chattingServer->setWindowTitle(tr("Chatting Server"));
    chattingServer->setWindowFlags(Qt::FramelessWindowHint);
    chattingServer->showMaximized();

    chattingClient = new ChattingClient(this);
    ui->mdiArea->addSubWindow(chattingClient);
    chattingClient->setWindowTitle(tr("Chatting Client"));
    chattingClient->setWindowFlags(Qt::FramelessWindowHint);
    chattingClient->showMaximized();

    connect(customerManage, SIGNAL(destroyed()),customerManage, SLOT(deleteLater()));
    connect(customerManage, SIGNAL(addedCustomer(int, QString)),purchaseManage, SLOT(addCustomer(int, QString)));
    connect(customerManage, SIGNAL(sendCustomerInfo(QString, QString, QString, QString, QString, QString, QString, QString)),purchaseManage,SLOT(getCustomerInfo(QString,QString,QString)));
    connect(customerManage, SIGNAL(addedCustomer(int, QString)),chattingServer,SLOT(addClient(int, QString)));

    connect(merchandiseManage, SIGNAL(destroyed()),merchandiseManage, SLOT(deleteLater()));
    connect(merchandiseManage, SIGNAL(addedMerchandise(int, QString)),purchaseManage, SLOT(addMerchandise(int, QString)));
    connect(merchandiseManage, SIGNAL(sendMerchandiseInfo(QString, int, int)),purchaseManage, SLOT(getMerchandiseInfo(QString, int, int)));

    connect(purchaseManage, SIGNAL(destroyed()),purchaseManage, SLOT(deleteLater()));
    connect(purchaseManage, SIGNAL(getCustomerInfo(int)),customerManage, SLOT(acceptCustomerInfo(int)));
    connect(purchaseManage, SIGNAL(getMerchandiseInfo(int)),merchandiseManage, SLOT(acceptMerchandiseInfo(int)));

    connect(chattingServer, SIGNAL(destroyed()), chattingServer, SLOT(deleteLater()));
    connect(chattingClient, SIGNAL(destroyed()), chattingClient, SLOT(deleteLater()));

    connect(customerSearch, SIGNAL(destroyed()),customerSearch, SLOT(deleteLater()));
    connect(merchandiseSearch, SIGNAL(destroyed()),merchandiseSearch, SLOT(deleteLater()));
    connect(purchaseSearch, SIGNAL(destroyed()),purchaseSearch, SLOT(deleteLater()));


    ui->mdiArea->setActiveSubWindow(cw);
}


void MainWindow::on_actionCustomer_triggered()
{
    if(customerManage != nullptr)
    {
        customerManage->setFocus();
    }
}

void MainWindow::on_actionMerchandise_triggered()
{
    if(merchandiseManage != nullptr)
    {
        merchandiseManage->setFocus();
    }
}

void MainWindow::on_actionPurchase_triggered()
{
    if(purchaseManage != nullptr)
    {
        purchaseManage->setFocus();
    }
}


void MainWindow::on_actionChatting_triggered()
{
    if(chattingServer != nullptr)
    {
        chattingServer->setFocus();
    }
}


void MainWindow::on_customerChatAction_triggered()
{
    if(chattingClient != nullptr)
    {
        chattingClient->setFocus();
    }
}


void MainWindow::on_customerSearchAction_triggered()
{
    if(customerSearch != nullptr)
    {
        customerSearch->setFocus();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

