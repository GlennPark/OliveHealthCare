#include "purchasemanage.h"
#include "ui_purchasemanage.h"
#include "purchaselist.h"

#include <QFile>
#include <QMenu>

PurchaseManage::PurchaseManage(QWidget *parent)
    : QWidget(parent), ui(new Ui::PurchaseManage)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 540 << 600;
//    ui->splitter->setSizes(sizes);

    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

}
void PurchaseManage::dataSave()
{
    QFile file("purchase.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");
        if(row.size()) {
            int id = row[0].toInt();
            QString cname = ui->customerComboBox->itemText(CidList.indexOf(row[1].toInt()));
            QString pname = ui->merchandiseComboBox->itemText(MidList.indexOf(row[2].toInt()));
            PurchaseList* c = new PurchaseList(id, row[1].toInt(), cname,
                    row[2].toInt(), pname, row[3].toInt(),
                    row[4].toInt(), row[5].toInt());
            ui->treeWidget->addTopLevelItem(c);
            purchaseList.insert(id, c);
        }
    }
    file.close( );
}

PurchaseManage::~PurchaseManage()
{
    delete ui;

    QFile file("purchase.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : purchaseList) {
        PurchaseList* o = v;
        out << o->id() << ", " << o->getCid() << ", ";
        out << o->getPid() << ", ";
        out << o->getPrice() << ", ";
        out << o->getQuantity() << ", ";
        out << o->getTotalPrice() << "\n";
    }
    file.close( );
}

void PurchaseManage::addCustomer(int id, QString name)
{
    CidList << id;
    ui->customerComboBox->addItem(name);
}

void PurchaseManage::addMerchandise(int id, QString name)
{
    MidList << id;
    ui->merchandiseComboBox->addItem(name);
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
    ui->toolBox->setCurrentIndex(0);
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

void PurchaseManage::on_addPushButton_clicked()
{
    int id = makePid( );
    ui->idLineEdit->setText(QString::number(id));
    int cid, pid, price, quantity, totalPrice;
    cid = CidList[ui->customerComboBox->currentIndex()];
    pid = MidList[ui->merchandiseComboBox->currentIndex()];
    price = ui->priceLineEdit->text().toInt();
    quantity = ui->quantitySpinBox->value();
    totalPrice = ui->totalPriceLineEdit->text().toInt();
    QString cname = ui->customerComboBox->itemText(CidList.indexOf(cid));
    QString pname = ui->merchandiseComboBox->itemText(MidList.indexOf(pid));
    if(ui->customerComboBox->currentIndex() >= 0) {
        PurchaseList* o = new PurchaseList(id, cid, cname, pid, pname,
                                     price, quantity, totalPrice);
        ui->treeWidget->addTopLevelItem(o);
        purchaseList.insert(id, o);
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
