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
                    "(cid INTEGER PRIMARY KEY, "
                    "(mid INTEGER PRIMARY KEY, "
                    "totalPrice VARCHAR(300), "
                    "shopDate Date);");
        pModel = new QSqlTableModel(this, ohcDB);
        pModel->setTable("purchase");
        pModel->select();

//        cModel = new QSqlTableModel(Custome, ohcDB);

        pModel->setHeaderData(0, Qt::Horizontal, tr("PID"));
        cModel->setHeaderData(1, Qt::Horizontal, tr("CID"));
        mModel->setHeaderData(2, Qt::Horizontal, tr("MID"));

    }
}

void PurchaseManage::on_addPushButton_clicked()
{
    int Pid = makePid( );
    ui->idLineEdit->setText(QString::number(Pid));
    int Cid, Mid, totalPrice;


    Mid = MidList[ui->merchandiseComboBox->currentIndex()];
    price = ui->priceLineEdit->text().toInt();
    quantity = ui->quantitySpinBox->value();
    totalPrice = ui->totalPriceLineEdit->text().toInt();
    QString cname = ui->customerComboBox->itemText(CidList.indexOf(cid));
    QString pname = ui->merchandiseComboBox->itemText(MidList.indexOf(pid));

    QSqlDatabase ohcDB = QSqlDatabase::database("purchaseConnection, customerConnection, merchandiseConnection");

    //    QSqlDatabase ohcDB = QSqlDatabase::database("customerConnection");

    if(ohcDB.isOpen() && pname.length()) {

        ui->tableView->resizeColumnsToContents();

    }
}

void PurchaseManage::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int key = item->text(0).toInt();
        PurchaseList* o = purchaseList[key];

        int cid, pid, price, quantity, totalPrice;
        cid = CidList[ui->customerComboBox->currentIndex()];
        pid = MidList[ui->merchandiseComboBox->currentIndex()];
        price = ui->priceLineEdit->text().toInt();
        quantity = ui->quantitySpinBox->value();
        totalPrice = ui->totalPriceLineEdit->text().toInt();
        o->setCid(cid);
        o->setPid(pid);
        o->setPrice(price);
        o->setQuantity(quantity);
        o->setTotalPrice(totalPrice);
        purchaseList[key] = o;
    }
}

void PurchaseManage::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
    //    for(int i = 0; i < ui->treeWidget->columnCount(); i++)
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i == 1 || i == 2)? Qt::MatchCaseSensitive|Qt::MatchContains
                                  : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            PurchaseList* o = static_cast<PurchaseList*>(i);
            int id = o->id();
            int cid = o->getCid();
            int pid = o->getPid();
            int price = o->getPrice();
            int quantity = o->getQuantity();
            int totalPrice = o->getTotalPrice();
            QString cname = ui->customerComboBox->itemText(CidList.indexOf(cid));
            QString pname = ui->merchandiseComboBox->itemText(MidList.indexOf(pid));
            PurchaseList* item = new PurchaseList(id, cid, cname, pid, pname,
                                                  price, quantity, totalPrice);
            ui->searchTreeWidget->addTopLevelItem(item);
        }
    }
}

void PurchaseManage::on_customerComboBox_currentIndexChanged(int index)
{
    emit getCustomerInfo(CidList[index]);
}

void PurchaseManage::getCustomerInfo(QString name, QString phoneNumber, QString address)
{
    ui->customerTreeWidget->clear();
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->customerTreeWidget);
    item->setText(0, name);
    item->setText(1, phoneNumber);
    item->setText(2, address);

}

void PurchaseManage::getMerchandiseInfo(QString name, int price, int quantity)     // 응답
{
    ui->merchandiseTreeWidget->clear();
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->merchandiseTreeWidget);
    item->setText(0, name);
    item->setText(1, QString::number(price));
    item->setText(2, QString::number(quantity));

    ui->priceLineEdit->setText(QString::number(price));

    //    QIntValidator *v = new QIntValidator(0, price, this);
    //    ui->quantityLineEdit->setValidator(v);
    ui->quantitySpinBox->setMaximum(quantity);
}

void PurchaseManage::on_merchandiseComboBox_currentIndexChanged(int index)
{
    emit getMerchandiseInfo(MidList[index]);
}

void PurchaseManage::on_quantitySpinBox_valueChanged(int arg1)
{
    ui->totalPriceLineEdit->setText(QString::number(arg1 * ui->priceLineEdit->text().toInt()));
}

void PurchaseManage::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        purchaseList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        //        delete item;
        ui->treeWidget->update();
    }
}

void PurchaseManage::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void PurchaseManage::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    emit getCustomerInfo(CidList[ui->customerComboBox->findText(item->text(1))]);
    emit getMerchandiseInfo(MidList[ui->merchandiseComboBox->findText(item->text(2))]);
    ui->idLineEdit->setText(item->text(0));
    ui->customerComboBox->setCurrentText(item->text(1));
    ui->merchandiseComboBox->setCurrentText(item->text(2));
    ui->priceLineEdit->setText(item->text(3));
    ui->quantitySpinBox->setValue(item->text(4).toInt());
    ui->totalPriceLineEdit->setText(item->text(5));

}

int PurchaseManage::makePid( )
{
    if(purchaseList.size( ) == 0) {
        return 300;
    } else {
        auto id = purchaseList.lastKey();
        return ++id;
    }
}



void PurchaseManage::addCustomer(int Cid)
{
    CidList << Cid;
    ui->customerComboBox->currentText();
}

void PurchaseManage::addMerchandise(int Mid)
{
    MidList << Mid;
    ui->merchandiseComboBox->currentText();
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
