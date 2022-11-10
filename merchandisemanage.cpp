#include "merchandisemanage.h"
#include "ui_merchandisemanage.h"
#include "merchandiselist.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>




MerchandiseManage::MerchandiseManage(QWidget* parent)
    : QWidget(parent), ui(new Ui::MerchandiseManage)
{
    ui->setupUi(this);

    QList<int> size;
    size << 600 << 600;
//    ui->splitter->setSizes(size);

    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

}

void MerchandiseManage::dataSave()
{
    QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","merchandiseConnection");
    ohcDB.setDatabaseName("merchandiseDatabase.db");

    if(ohcDB.open())
    {
        QSqlQuery mQuery(ohcDB);
        mQuery.exec("CREATE TABLE IF NOT EXISTS merchandise"
                    "(pid INTEGER PRIMARY KEY,"
                    "pname VARCHAR(20), "
                    "price VARCHAR(100), "
                    "quantity VARCHAR(100), "
                    "madein VARCHAR(100), "
                    "category VARCHAR(100), "
                    "description VARCHAR(500), "
                    "enrollDate DATE);");

        mModel = new QSqlTableModel(this, ohcDB);
        mModel->setTable("merchandise");
        mModel->select();

        mModel->setHeaderData(0, Qt::Horizontal, tr("MID"));
        mModel->setHeaderData(1, Qt::Horizontal, tr("PNAME"));
        mModel->setHeaderData(2, Qt::Horizontal, tr("PRICE"));
        mModel->setHeaderData(3, Qt::Horizontal, tr("QUANTITY"));
        mModel->setHeaderData(4, Qt::Horizontal, tr("MADEIN"));
        mModel->setHeaderData(5, Qt::Horizontal, tr("CATEGORY"));
        mModel->setHeaderData(6, Qt::Horizontal, tr("DESCRIPTION"));
        mModel->setHeaderData(7, Qt::Horizontal, tr("ENROLLDATE"));

    }
    ui->tableView->setModel(mModel);
    ui->tableView->resizeColumnsToContents()

    for(int i = 0; i <mModel->rowCount(); i++)
    {
        int Mid = mModel->data(mModel->index(i, 0)).toInt();
        QString pname = mModel->data(mModel->index(i, 1)).toString();
        QString price = mModel->data(mModel->index(i, 2)).toString();
        QString quantity = mModel->data(mModel->index(i, 3)).toString();
        QString madein = mModel->data(mModel->index(i, 4)).toString();
        QString category = mModel->data(mModel->index(i, 5)).toString();
        QString description = mModel->data(mModel->index(i, 6)).toString();
        QString enrolldate = mModel->data(mModel->index(i, 7)).toString();

        emit addedMerchandise(Mid);
    }


}

void MerchandiseManage::on_addPushButton_clicked()
{
    int Mid = makeMid( );
    QString pname, price, quantity, madein, category, description, enrolldate;

    ui->idLineEdit->setText(QString::number(id));

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
//    ui->toolBox->setCurrentIndex(0);
}

void MerchandiseManage::acceptMerchandiseInfo(int key)
{
    MerchandiseList* p = merchandiseList[key];
    emit sendMerchandiseInfo(p->getName(), p->getPrice(), p->getQuantity());
}

void MerchandiseManage::on_favoriteTabWidget_tabBarClicked(int index)
{

}


void MerchandiseManage::on_columnView_clicked(QModelIndex &index)
{

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
