#include "merchandisemanage.h"
#include "ui_merchandisemanage.h"
#include "merchandiselist.h"

#include <QFile>
#include <QMenu>

MerchandiseManage::MerchandiseManage(QWidget* parent)
    : QWidget(parent), ui(new Ui::MerchandiseManage)
{
    ui->setupUi(this);

    QList<int> size;
    size << 600 << 600;
    ui->splitter->setSizes(size);

    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

}

void MerchandiseManage::dataSave()
{
    QFile file("merchandise.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");
        if(row.size()) {
            int id = row[0].toInt();
            MerchandiseList* c = new MerchandiseList(id, row[1], row[2].toInt(), row[3].toInt());
            ui->treeWidget->addTopLevelItem(c);
            merchandiseList.insert(id, c);
            emit addedMerchandise(id, row[1]);
        }
    }
    file.close( );
}

MerchandiseManage::~MerchandiseManage()
{
    delete ui;

    QFile file("merchandise.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : merchandiseList) {
        MerchandiseList* c = v;
        out << c->id() << ", " << c->getName() << ", ";
        out << c->getPrice() << ", ";
        out << c->getQuantity() << "\n";
    }
    file.close( );
}

int MerchandiseManage::makeid( )
{
    if(merchandiseList.size( ) == 0) {
        return 200;
    } else {
        auto id = merchandiseList.lastKey();
        return ++id;
    }
}

void MerchandiseManage::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        merchandiseList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
//        delete item;
        ui->treeWidget->update();
    }
}

void MerchandiseManage::on_addPushButton_clicked()
{
    int id = makeid( );
    ui->idLineEdit->setText(QString::number(id));
    QString name;
    int price, quantity;
    name = ui->nameLineEdit->text();
    price = ui->priceLineEdit->text().toInt();
    quantity = ui->quantitySpinBox->value();
    if(name.length()) {
        MerchandiseList* c = new MerchandiseList(id, name, price, quantity);
        ui->treeWidget->addTopLevelItem(c);
        merchandiseList.insert(id, c);
        emit addedMerchandise(id, name);
    }
}

void MerchandiseManage::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int key = item->text(0).toInt();
        MerchandiseList* c = merchandiseList[key];
        QString name;
        int price, quantity;
        name = ui->nameLineEdit->text();
        price = ui->priceLineEdit->text().toInt();
        quantity = ui->quantitySpinBox->value();
        c->setName(name);
        c->setPrice(price);
        c->setQuantity(quantity);
        merchandiseList[key] = c;
    }
}

void MerchandiseManage::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void MerchandiseManage::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
//    for(int i = 0; i < ui->treeWidget->columnCount(); i++)
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);
        foreach(auto i, items) {
            MerchandiseList* c = static_cast<MerchandiseList*>(i);
            int id = c->id();
            QString name = c->getName();
            int price = c->getPrice();
            int quantity = c->getQuantity();

            MerchandiseList* item = new MerchandiseList(id, name, price, quantity);
            ui->searchTreeWidget->addTopLevelItem(item);
        }
    }
}

void MerchandiseManage::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    ui->idLineEdit->setText(item->text(0));
    ui->nameLineEdit->setText(item->text(1));
    ui->priceLineEdit->setText(item->text(2));
    ui->quantitySpinBox->setValue(item->text(3).toInt());
    ui->toolBox->setCurrentIndex(0);
}

void MerchandiseManage::acceptMerchandiseInfo(int key)
{
    MerchandiseList* p = merchandiseList[key];
    emit sendMerchandiseInfo(p->getName(), p->getPrice(), p->getQuantity());
}
