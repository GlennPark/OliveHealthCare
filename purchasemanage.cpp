#include "purchasemanage.h"
#include "ui_purchasemanage.h"
#include "purchaselist.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QStandardItemModel>

PurchaseManage::PurchaseManage(QWidget *parent)
    : QWidget(parent), ui(new Ui::PurchaseManage)
{
    ui->setupUi(this);

    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

    sModel = new QStandardItemModel(0, 8);
    sModel->setHeaderData(0, Qt::Horizontal, tr("PID"));
    sModel->setHeaderData(1, Qt::Horizontal, tr("CID"));
    sModel->setHeaderData(2, Qt::Horizontal, tr("MID"));
    sModel->setHeaderData(3, Qt::Horizontal, tr("NAME"));
    sModel->setHeaderData(4, Qt::Horizontal, tr("PNAME"));
    sModel->setHeaderData(5, Qt::Horizontal, tr("FAVORITE"));
    sModel->setHeaderData(6, Qt::Horizontal, tr("CATEGORY"));
    sModel->setHeaderData(7, Qt::Horizontal, tr("PRICE"));
    sModel->setHeaderData(8, Qt::Horizontal, tr("QUANTITY"));
    sModel->setHeaderData(9, Qt::Horizontal, tr("BUYAMOUNT"));
    sModel->setHeaderData(10, Qt::Horizontal, tr("TOTALPRICE"));
    sModel->setHeaderData(11, Qt::Horizontal, tr("SHOPDATE"));

    ui->searchTableView->setModel(sModel);

    clientInfoModel=new QStandardItemModel;
    clientInfoModel->setColumnCount(2);
    clientInfoModel->setRowCount(5);
    QStringList ls;
    ls<<"id"<<"phone";
    clientInfoModel->setHorizontalHeaderLabels(ls);


    ui->tableView->setModel(clientInfoModel);
   ui->tableView->show();
}

void PurchaseManage::dataSave()
{
    QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","purchaseConnection");
    ohcDB.setDatabaseName("purchaseDatabase.db");

    if(ohcDB.open())
    {
        QSqlQuery pQuery(ohcDB);
        pQuery.exec("CREATE TABLE IF NOT EXISTS purchase"
                    "(pid INTEGER PRIMARY KEY, "
                    "cid INTEGER PRIMARY KEY, "
                    "mid INTEGER PRIMARY KEY, "
                    "name varchar(20), "
                    "mname varchar(20), "
                    "favorite varchar(100), "
                    "category varchar(100), "
                    "price INTEGER, "
                    "quantity INTEGER, "
                    "buyAmount INTEGER, "
                    "totalPrice INTEGER, "
                    "shopDate Date);");

        pModel = new QSqlTableModel(this, ohcDB);
        pModel->setTable("purchase");
        pModel->setTable("customer");
        pModel->setTable("merchandise");
        pModel->select();

        //        cModel = new QSqlTableModel(Custome, ohcDB);

        pModel->setHeaderData(0, Qt::Horizontal, tr("PID"));
        pModel->setHeaderData(1, Qt::Horizontal, tr("CID"));
        pModel->setHeaderData(2, Qt::Horizontal, tr("MID"));
        pModel->setHeaderData(3, Qt::Horizontal, tr("NAME"));
        pModel->setHeaderData(4, Qt::Horizontal, tr("MNAME"));
        pModel->setHeaderData(5, Qt::Horizontal, tr("FAVORITE"));
        pModel->setHeaderData(6, Qt::Horizontal, tr("CATEGORY"));
        pModel->setHeaderData(7, Qt::Horizontal, tr("PRICE"));
        pModel->setHeaderData(8, Qt::Horizontal, tr("QUANTITY"));
        pModel->setHeaderData(9, Qt::Horizontal, tr("BUYAMOUNT"));
        pModel->setHeaderData(10, Qt::Horizontal, tr("TOTALPRICE"));
        pModel->setHeaderData(11, Qt::Horizontal, tr("SHOPDATE"));
    }
    ui->tableView->setModel(pModel);
    ui->tableView->resizeColumnsToContents();

    for(int i = 0; i < pModel->rowCount(); i++)
    {
        int Pid = pModel->data(pModel->index(i, 0)).toInt();
        int Cid = pModel->data(pModel->index(i, 1)).toInt();
        int Mid = pModel->data(pModel->index(i, 2)).toInt();
        QString name = pModel->data(pModel->index(i, 3)).toString();
        QString mname = pModel->data(pModel->index(i, 4)).toString();
        QString favorite = pModel->data(pModel->index(i, 5)).toString();
        QString category = pModel->data(pModel->index(i, 6)).toString();
        int price = pModel->data(pModel->index(i, 7)).toInt();
        int quantity = pModel->data(pModel->index(i, 8)).toInt();
        int buyAmount = pModel->data(pModel->index(i, 9)).toInt();
        int totalPrice = pModel->data(pModel->index(i, 10)).toInt();
        QString shopDate = pModel->data(pModel->index(i, 11)).toString();

        emit addedPurchase(Pid);

    }
}

void PurchaseManage::on_addPushButton_clicked()
{
    int Pid = makePid( );
    int Cid, Mid, price, quantity, buyAmount, totalPrice;
    QString name, mname, favorite, category, shopDate;
    ui->PidLineEdit->setText(QString::number(Pid));
    ui->CidLineEdit->setText(QString::number(Cid));
    ui->MidLineEdit->setText(QString::number(Mid));


    name = ui->nameComboBox->currentText();
    mname = ui->mnameComboBox->currentText();
    favorite = ui->favoriteComboBox->currentText();
    category = ui->categoryComboBox->currentText();
    price = ui->priceLineEdit->text().toInt();
    quantity = ui->quantitySpinBox->value();
    buyAmount = ui->buyAmountSpinBox->value();
    totalPrice = ui->totalPriceLineEdit->text().toInt();
    shopDate = ui->shopDateEdit->date().toString();

    QSqlDatabase ohcDB = QSqlDatabase::database("purchaseConnection, customerConnection, merchandiseConnection");

    if(ohcDB.isOpen() && name.length())
    {
        QSqlQuery pQuery(pModel->database());
        pQuery.prepare("INSERT INTO purchase VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        pQuery.bindValue(0, Pid);
        pQuery.bindValue(1, Cid);
        pQuery.bindValue(2, Mid);
        pQuery.bindValue(3, name);
        pQuery.bindValue(4, mname);
        pQuery.bindValue(5, favorite);
        pQuery.bindValue(6, category);
        pQuery.bindValue(7, price);
        pQuery.bindValue(8, quantity);
        pQuery.bindValue(9, buyAmount);
        pQuery.bindValue(10, totalPrice);
        pQuery.bindValue(11, shopDate);

        pQuery.exec();
        pModel->select();
        ui->tableView->resizeColumnsToContents();
        emit(addedPurchase(Pid));
    }
}

void PurchaseManage::on_modifyPushButton_clicked()
{
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        // int Pid = makePid( );

        int Cid, Mid, price, quantity, buyAmount, totalPrice;
        QString name, mname, favorite, category, shopDate;

        name = ui->nameComboBox->currentText();
        mname = ui->mnameComboBox->currentText();
        favorite = ui->favoriteComboBox->currentText();
        category = ui->categoryComboBox->currentText();
        price = ui->priceLineEdit->text().toInt();
        quantity = ui->quantitySpinBox->value();
        buyAmount = ui->buyAmountSpinBox->value();
        totalPrice = ui->totalPriceLineEdit->text().toInt();
        shopDate = ui->shopDateEdit->date().toString();


#if 1
        //        pModel->setData(modelIndex.siblingAtColumn(0), Pid);
        //        pModel->setData(modelIndex.siblingAtColumn(1), Cid);
        //        pModel->setData(modelIndex.siblingAtColumn(2), Mid);

        pModel->setData(modelIndex.siblingAtColumn(3), name);
        pModel->setData(modelIndex.siblingAtColumn(4), mname);
        pModel->setData(modelIndex.siblingAtColumn(5), favorite);
        pModel->setData(modelIndex.siblingAtColumn(6), category);
        pModel->setData(modelIndex.siblingAtColumn(7), price);
        pModel->setData(modelIndex.siblingAtColumn(8), quantity);
        pModel->setData(modelIndex.siblingAtColumn(9), buyAmount);
        pModel->setData(modelIndex.siblingAtColumn(10), totalPrice);
        pModel->setData(modelIndex.siblingAtColumn(11), shopDate);
        pModel->submit();

#else
        QSqlQuery pQuery(pModel->database());
        pQuery.prepare("UPDATE purchase SET name = ?, mname = ?, favorite = ?, category = ?, price = ?, quantity = ?, buyAmount = ?, totalPrice = ?, shopDate = ?");

        pQuery.bindValue(0, name);
        pQuery.bindValue(1, mname);
        pQuery.bindValue(2, favorite);
        pQuery.bindValue(3, category);
        pQuery.bindValue(4, price);
        pQuery.bindValue(5, quantity);
        pQuery.bindValue(6, buyAmount);
        pQuery.bindValue(7, totalPrice);
        pQuery.bindValue(8, shopDate);
        pQuery.bindValue(9, Pid);
        pQuery.bindValue(10, Cid);
        pQuery.bindValue(11, Mid);
        pQuery.exec();
#endif
        pModel->select();
        ui->tableView->resizeColumnsToContents();

    }
}





void PurchaseManage::on_searchPushButton_clicked()
{
    sModel->clear();
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    QModelIndexList indexes = pModel->match(pModel->index(0, i), Qt::EditRole, ui->searchLineEdit->text(), -1, Qt::MatchFlags(flag));

    foreach(auto k, indexes)
    {
        int Pid = pModel->data(k.siblingAtColumn(0)).toInt();
        int Cid = pModel->data(k.siblingAtColumn(1)).toInt();
        int Mid = pModel->data(k.siblingAtColumn(2)).toInt();
        QString name = pModel->data(k.siblingAtColumn(3)).toString();
        QString mname = pModel->data(k.siblingAtColumn(4)).toString();
        QString favorite = pModel->data(k.siblingAtColumn(5)).toString();
        QString category = pModel->data(k.siblingAtColumn(6)).toString();
        int price = pModel->data(k.siblingAtColumn(7)).toInt();
        int quantity = pModel->data(k.siblingAtColumn(8)).toInt();
        int buyAmount = pModel->data(k.siblingAtColumn(9)).toInt();
        int totalPrice = pModel->data(k.siblingAtColumn(10)).toInt();
        QString shopDate = pModel->data(k.siblingAtColumn(11)).toString();

        QStringList stringList;

        stringList << QString::number(Pid) << QString::number(Cid) << QString::number(Mid) << name << mname << favorite << category << QString::number(price) << QString::number(quantity) << QString::number(buyAmount) << QString::number(totalPrice) << shopDate ;

        QList<QStandardItem *> items;
        for (int i = 0; i < 12; ++i) {
            items.append(new QStandardItem(stringList.at(i)));
        }

        sModel->appendRow(items);
        ui->searchTableView->resizeColumnsToContents();
    }
}

int PurchaseManage::makePid( )
{
    if(pModel->rowCount() == 0) {
        return 0;
    } else {
        auto Pid = pModel->data(pModel->index(pModel->rowCount()-1, 0)).toInt();
        return ++Pid;
    }
}

void PurchaseManage::removeItem()
{
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        pModel->removeRow(modelIndex.row());
        pModel->select();
        ui->tableView->resizeColumnsToContents();
    }
}

void PurchaseManage::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

void PurchaseManage::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    if(ui->tableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

void PurchaseManage::acceptPurchaseInfo(int key)     // 응답
{
    QModelIndexList indexList =
            pModel->match(pModel->index(0, 0), Qt::EditRole, key, -1, Qt::MatchFlags(Qt::MatchCaseSensitive));

    foreach(auto k, indexList)
    {
        //  int Pid = pModel->data(k.siblingAtColumn(0)).toInt();
        //  int Cid = pModel->data(k.siblingAtColumn(1)).toInt();
        //  int Mid = pModel->data(k.siblingAtColumn(2)).toInt();
        QString name = pModel->data(k.siblingAtColumn(3)).toString();
        QString mname = pModel->data(k.siblingAtColumn(4)).toString();
        QString favorite = pModel->data(k.siblingAtColumn(5)).toString();
        QString category = pModel->data(k.siblingAtColumn(6)).toString();
        int price = pModel->data(k.siblingAtColumn(7)).toInt();
        int quantity = pModel->data(k.siblingAtColumn(8)).toInt();
        int buyAmount = pModel->data(k.siblingAtColumn(9)).toInt();
        int totalPrice = pModel->data(k.siblingAtColumn(10)).toInt();
        QString shopDate = pModel->data(k.siblingAtColumn(11)).toString();

        emit sendPurchaseInfo(name, mname, favorite, category, price, quantity, buyAmount, totalPrice, shopDate);

    }
}

void PurchaseManage::on_tableView_clicked(const QModelIndex &index)
{
    QString Pid = pModel->data(index.siblingAtColumn(0)).toString();
    QString Cid = pModel->data(index.siblingAtColumn(1)).toString();
    QString Mid = pModel->data(index.siblingAtColumn(2)).toString();
    QString name = pModel->data(index.siblingAtColumn(3)).toString();
    QString mname = pModel->data(index.siblingAtColumn(4)).toString();
    QString favorite = pModel->data(index.siblingAtColumn(5)).toString();
    QString category = pModel->data(index.siblingAtColumn(6)).toString();
    QString price = pModel->data(index.siblingAtColumn(7)).toString();
    int quantity = pModel->data(index.siblingAtColumn(8)).toInt();
    int buyAmount = pModel->data(index.siblingAtColumn(9)).toInt();
    QString totalPrice = pModel->data(index.siblingAtColumn(10)).toString();
    QDate shopDate = pModel->data(index.siblingAtColumn(11)).toDate();

    ui->PidLineEdit->setText(Pid);
    ui->CidLineEdit->setText(Cid);
    ui->MidLineEdit->setText(Mid);
    ui->nameComboBox->setCurrentText(name);
    ui->mnameComboBox->setCurrentText(mname);
    ui->favoriteComboBox->setCurrentText(favorite);
    ui->categoryComboBox->setCurrentText(category);
    ui->priceLineEdit->setText(price);
    ui->quantitySpinBox->setValue(quantity);
    ui->buyAmountSpinBox->setValue(buyAmount);
    ui->totalPriceLineEdit->setText(totalPrice);
    ui->shopDateEdit->setDate(shopDate);


}
void PurchaseManage::on_nameComboBox_currentIndexChanged(int index)
{
    qDebug()<<"1104";
    emit getCustomerInfo(index);
}

void PurchaseManage::on_mnameComboBox_currentIndexChanged(int index)
{
    //emit getMerchandiseInfo(MidList[index]);
}

//void PurchaseManage::getCustomerInfo(QString name, QString phoneNumber, QString address)
//{
//    ui->customerTreeWidget->clear();
//    QTreeWidgetItem *item = new QTreeWidgetItem(ui->customerTreeWidget);
//    item->setText(0, name);
//    item->setText(1, phoneNumber);
//    item->setText(2, address);

//}




void PurchaseManage::on_quantitySpinBox_valueChanged(int arg1)
{
    ui->totalPriceLineEdit->setText(QString::number(arg1 * ui->priceLineEdit->text().toInt()));
}


void PurchaseManage::addCustomer(int Cid)
{
        //CidList << Cid;
     qDebug()<<"d";
        ui->nameComboBox->addItem(QString::number(Cid));

}

void PurchaseManage::addCustomer(int id, QString){

    ui->nameComboBox->addItem(QString::number(id));

}

void PurchaseManage::addMerchandise(int Mid)
{
       // MidList << Mid;

        ui->mnameComboBox->addItem(QString::number(Mid));
}
PurchaseManage::~PurchaseManage()
{
    delete ui;
    QSqlDatabase database = QSqlDatabase::database("purchaseConnection");

    if(database.isOpen())
    {
        pModel->submitAll();
        database.commit();
        database.close();
    }
}
void PurchaseManage::acceptCustomerInfo(QString name, QString phoneNumber, QString, QString, QString, QString, QString, QString){
    qDebug()<<"accept";
    QStandardItem* item_name = new QStandardItem;
    item_name->setText(name);

    QStandardItem* item_phone = new QStandardItem;
    item_phone->setText(phoneNumber);

    QList<QStandardItem*> ls;
    ls<<item_name<<item_phone;
    clientInfoModel->setItem(1,1,item_name);
ui->tableView->update();

}
